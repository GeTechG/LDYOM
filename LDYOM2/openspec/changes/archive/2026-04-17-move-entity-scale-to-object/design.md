## Context

Сейчас `Entity` хранит `std::array<float, 3> scale = {1,1,1}` и пару `getScaleCallback`/`setScaleCallback`, которые компоненты подключают в своём `onStart`. Текущая реализация применения scale — утилита `scaleMatrix(CMatrix&, array<float,3>)`, которая умножает столбцы матрицы сущности (`matrix.right/up/at *= scale[i]`) и пушит результат в RenderWare через `UpdateRW`. Эта схема применяется в компонентах `Actor`, `Vehicle`, `Object`, `Pickup`, `Particle` и в in-game transform editor.

Проблемы текущего дизайна:

1. **Нарушение ортонормальности матрицы** — `CEntity::m_matrix` у GTA всегда ожидается ортонормальным (единичные векторы right/up/at + позиция). Умножение столбцов ломает это инвариант. Последствия: некорректные рейкасты, дрейф в интеграции физики, поломанные анимации у `CPed`.
2. **Конфликт с `CObject::PreRender`** — движок каждый кадр выполняет `RwMatrixScale(GetModellingMatrix(), vec(m_fScale, m_fScale, m_fScale), rwCOMBINEPRECONCAT)`. Наше ручное изменение матрицы и игровой `RwMatrixScale` накладываются друг на друга непредсказуемо, особенно после `UpdateRwMatrix`, который затирает наши правки.
3. **Фиктивная поддержка у актёров и машин** — ped и vehicle — скелетные/составные объекты. Визуальный эффект ручного скейла у них есть, но не стабилен и физически бессмысленен. Поддерживать это как публичный редактор-контракт — ложное обещание.
4. **Не-uniform scale невозможен в движке** — ванильный `m_fScale` — один `float`. Наш `float[3]` создаёт иллюзию per-axis scaling, не отражаемую ни в физике, ни в нативном рендере.

Сам редизайн инициирован как Phase 1 более крупной задачи «сделать scale физичным». Phase 1 — выровнять поверхность под реальные возможности движка; последующие фазы (коллизия, per-axis, vehicle/actor scale) — отдельные changes.

Относится к: `CObject`, `CPed`, `CVehicle`, `CPhysical`, `CMatrix`, `CMatrixLink` — классы из plugin-sdk. Подтверждённый реверс `CObject::PreRender` и `CObject::Init` находится в `C:\Users\User\Documents\gta-reversed\source\game_sa\Entity\Object\Object.cpp`.

## Goals / Non-Goals

**Goals:**

- Привести модель данных LDYOM к соответствию тому, что движок действительно умеет: uniform scale и только у объектов.
- Использовать нативный `CObject::m_fScale` как единственный путь применения scale. Не дублировать работу игры.
- Убрать ручные mutation матрицы сущности для scale — чтобы не нарушать ортонормальность.
- Сохранить совместимость JSON со старыми сохранениями через молчаливую миграцию (без алерта пользователю).
- Удалить поле `scale` из `Entity` и связанные колбэки — не держать «мёртвый» API.
- Обновить Lua декларации и биндинги.

**Non-Goals:**

- **Физический скейл коллизии** (`CColModel` клонирование, scale `m_boundBox`/`m_boundSphere`, `m_pColData`) — это Phase 2.
- **Scale актёров через скелет** (`RpHAnimHierarchy` root bone) — не в этом change.
- **Scale машин** (требует пересчёта `handling.cfg`, иерархии компонентов) — не в этом change.
- **Per-axis (non-uniform) scale** — не в этом change. Движок его не поддерживает; если потребуется в будущем — отдельная задача.
- **Предупреждение пользователю о миграции** (баннер «scale был перенесён»). Решено молча.
- **Ограничение range scale** (min/max) — пользователь явно отказался. Отрицательные и нулевые значения допустимы.

## Decisions

### Decision 1: Перенести `scale` из `Entity` в компонент `Object`, не оставлять в `Entity`

**Выбрано**: полностью удалить `scale`, `getScaleCallback`, `setScaleCallback` из `Entity`. Добавить `float scale = 1.0f` в `Object`.

**Альтернативы**:

- *Оставить `scale` в `Entity` как `float`, но игнорировать для не-объектов.* Минус: оставляет поле, которое для большинства сущностей ничего не делает — инвайт для будущих багов и недоразумений. Плюс: меньше рефакторинга transform-API.
- *Оставить `std::array<float, 3>` и просто не показывать UI.* Минус: данные продолжают врать о возможностях движка. Плюс: нулевой рефакторинг.

**Обоснование выбора**: scale — это реально свойство только объектов в GTA SA. Хранение в `Object` делает domain-модель честной и заодно проясняет, что компоненты `Actor`/`Vehicle` scale не имеют — не по недосмотру, а принципиально. Стоимость рефакторинга — одна смена сигнатуры у `setGetTransformCallbacks`/`setSetTransformCallbacks` и протяжка изменения через все компоненты, ~13 файлов. Приемлемо.

### Decision 2: Применять scale через `CObject::m_fScale`, а не через прямую матрицу

**Выбрано**: `object->m_fScale = this->scale;` в `spawn` и при изменении (`dirty & Scale` в `onUpdate`). Никаких `scaleMatrix` вызовов.

**Альтернативы**:

- *Продолжать ручной `scaleMatrix` после `PreRender`.* Это требует хука рендер-цикла и борьбы с игрой. Хрупко.
- *Хуковать `CObject::PreRender` самим.* Лишняя инвазия, ничего не даёт сверх `m_fScale`.

**Обоснование**: `m_fScale` — документированный, стабильный механизм, используемый опкодом 07A8 (`SET_OBJECT_SCALE`). Игра сама применит его корректно каждый кадр. Это наименьшая инвазия с максимальной совместимостью.

### Decision 3: Миграция JSON — в `Entity::from_json`, pass-through через временное поле

**Выбрано**: при `Entity::from_json`, если найден ключ `"scale"` (старый формат), прочитать `scale[0]` во временную переменную-член класса `_legacyScale` (или передать через optional в процесс загрузки компонентов). После парсинга компонентов, если есть `Object`, записать туда значение; иначе — отбросить.

Формально: в структуре парсинга scene / entity нужно либо:
- (a) сохранить legacy в `Entity` как `std::optional<float> _pendingLegacyScale`, после десериализации компонентов — перекинуть в `Object`, потом обнулить;
- (b) парсить компоненты внутри `Entity::from_json` и при встрече `"object"` сразу применять legacy.

**Обоснование выбора варианта (a)**: отделяет ответственность — `Entity` не должен знать внутренности компонентов при парсинге, кроме этого одного частного случая. Временное поле (не сериализуется, помечено комментарием как миграция Phase 1) — цена низкая.

**При сериализации**: ключ `"scale"` в `Entity` никогда не пишется в новом коде. В `Object::to_json` добавляется `j["scale"] = scale`.

### Decision 4: Сигнатура transform-колбэков

**Выбрано**: убрать scale из обеих сигнатур:

```cpp
void setGetTransformCallbacks(std::function<std::array<float,3>()> pos,
                              std::function<CQuaternion()> rot);
void setSetTransformCallbacks(std::function<void(std::array<float,3>)> pos,
                              std::function<void(CQuaternion)> rot);
```

**Альтернатива**: оставить старую сигнатуру и передавать пустой колбэк для scale у не-объектов, в `Object` — рабочий. Минус: API продолжает обещать scale в общей transform-модели. Отвергнуто.

### Decision 5: Сброс `m_fScale = 1.0f` при despawn

**Выбрано**: в `Object::despawn` перед возвратом `CObject` в игровой пул — записать `m_fScale = 1.0f`.

**Обоснование**: `CObject` — пулированный ресурс. Если не сбросить, следующий `CObject::Init` после `GTAPool::New` установит `m_fScale = 1.0f` (см. `CObject::Init` в `gta-reversed`), так что теоретически это перекрывается. Но есть reuse-пути в игре, где `Init` не вызывается; для безопасности лучше явно сбрасывать.

### Decision 6: Dirty flag для scale

**Выбрано**: расширить существующий enum `DirtyFlags` у `Object` значением `Scale`. При изменении `scale` из UI/Lua — помечать `dirty |= Scale`, в `onUpdate` — если `dirty & Scale` — `object->m_fScale = scale`.

**Обоснование**: консистентно с существующим паттерном (`Rotation`, `Position`, `Model`).

### Decision 7: Удаление `src/utils/matrix_utils.h`

**Выбрано**: файл удаляется полностью, все `#include "matrix_utils.h"` вычищаются.

**Проверка**: grep подтверждает — после правок `scaleMatrix` нигде не вызывается.

## Risks / Trade-offs

- **[Риск]** Lua-аддоны пользователей могут ссылаться на `entity.scale`. → **Mitigation**: grep по `../ingame/addons/core`; если найдутся — переписать в рамках этого change. Для сторонних аддонов документируем BREAKING в changelog.
- **[Риск]** Миграция non-uniform scale (`[3, 1, 1]`) беззвучно теряет информацию. → **Mitigation**: устаревший формат был визуально-сломан — значения `scale[1]`/`scale[2]` ничего не давали в реальности, терять нечего. Можно залогировать warning в spdlog без UI-алерта.
- **[Риск]** Где-то код мог полагаться на side-effect ручного `scaleMatrix` (например, сломанная матрица использовалась для чего-то ещё). → **Mitigation**: тестирование существующих проектов после миграции. Никаких таких завязок в репо найти не удалось.
- **[Trade-off]** Удаление scale у actor/vehicle — пользователь, настраивавший визуальный скейл актёра (даже если он был глючный), потеряет эффект. → Осознанное решение: Phase 1 честно говорит «этого нельзя», Phase 2 может вернуть через скелет/коллизию.
- **[Риск]** Изменение сигнатуры `setGetTransformCallbacks`/`setSetTransformCallbacks` — ломает все компоненты, использующие её. → **Mitigation**: это in-repo API, все call-sites известны и обновляются в этом же change.

## Migration Plan

**Порядок правки** (важен — ломать API-сигнатуру нужно в синхронизированной пачке):

1. Расширить `Object::DirtyFlags` → добавить `Scale`.
2. Добавить `float scale` в `Object.h`, сериализацию в `Object::to_json`/`from_json`, UI в `editorRender`.
3. Реализовать применение `scale` → `m_fScale` в `Object::spawn`, `Object::onUpdate`, сброс в `Object::despawn`.
4. Изменить сигнатуры `setGetTransformCallbacks`/`setSetTransformCallbacks` в `Entity.h/cpp`. Убрать scale-поле и колбэки.
5. Обновить все компоненты, вызывающие изменённые сеттеры: убрать scale-лямбду. Одним махом.
6. Обновить `object_move_by_path.cpp` / `object_trigger_move.cpp`: читать scale из `Object`-компонента entity (получить через `entity->getComponent(Object::TYPE)`), вместо `entity->scale`.
7. Обновить `object_transform_editing.cpp`: `m_scale` → `float`; применение через `m_fScale`, не через `scaleMatrix`.
8. Добавить миграцию в `Entity::from_json`: поймать старый `scale`, положить в временное `std::optional<float> _legacyScale`. В `ScenesManager` / месте парсинга компонентов — после создания `Object`-компонента, если `_legacyScale.has_value()` — записать туда.
9. Удалить `src/utils/matrix_utils.h` и все `#include`.
10. Обновить Lua биндинги `Object::sol_lua_register` — добавить `scale`. Убрать scale из `Entity::sol_lua_register`, если был.
11. Обновить Lua декларации в `../ingame/definitions/core`.
12. Обновить локализацию `../ingame/languages/en.json` (ключ для UI-лейбла `scale` в Object).
13. Пройтись по `../ingame/addons/core`, найти `.scale` упоминания — при необходимости переписать.

**Rollback**: единичный git revert — изменение плотное, с чётким скоупом. Нет data-migrations в необратимом виде: старый JSON при откате будет читаться старым кодом нормально.

## Open Questions

- **Логирование миграции**: писать ли `spdlog::info("Migrated legacy entity.scale to Object.scale")` при загрузке старого проекта? Мелочь, но может помочь при диагностике. *Предложение*: да, debug-уровень.
- **Scale в in-game transform editor** — показывать ли вообще виджет scale в `ObjectTransformEditing` UI, или управление только через основной редактор компонента? *Предложение*: показывать — это же именно transform редактор объекта, scale там уместен.
- **Lua back-compat shim** — добавить ли deprecated-warning при попытке чтения `entity.scale` из Lua? *Предложение*: не нужно. `nil` — достаточно явный сигнал.
