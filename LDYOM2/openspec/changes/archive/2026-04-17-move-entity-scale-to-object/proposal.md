## Why

`Entity.scale` хранится как `std::array<float, 3>` и применяется через прямое умножение столбцов матрицы (`matrix.right/up/at *= scale`) для всех типов сущностей — актёров, машин, объектов, пикапов, партиклов. Это работает только визуально (физика GTA коллизии не масштабирует в принципе), ломает ортонормальность матрицы у `CPed`/`CVehicle`/`CObject` и конфликтует с собственным механизмом `CObject::PreRender`, который каждый кадр применяет свой `m_fScale` через `RwMatrixScale`. Для актёров и машин само понятие масштаба корректно работать не может без клонирования скелета/коллизии — их текущая поддержка «визуальная», но нестабильная. Нужен честный редизайн: scale — это свойство объекта, не каждой сущности.

## What Changes

- **BREAKING**: удалить `scale`, `getScaleCallback`, `setScaleCallback` из `Entity`. Сигнатуры `setGetTransformCallbacks`/`setSetTransformCallbacks` больше не принимают scale.
- **BREAKING**: убрать поддержку scale для компонентов `Actor`, `Vehicle`, `Pickup`, `Particle`, `Firework`, `Checkpoint`, `Train` — поля/колбэки/UI.
- Добавить `float scale = 1.0f` в компонент `Object`. Применять его через нативный `CObject::m_fScale` — игра сама выполнит `RwMatrixScale` в `PreRender`.
- UI для scale рендерится только в компоненте `Object` (числовой input без ограничений, negative/zero тоже допускаются).
- `object_move_by_path` и `object_trigger_move` читают scale из компонента `Object`, а не из `entity`.
- Удалить `src/utils/matrix_utils.h` (`scaleMatrix`) — больше не используется.
- `object_transform_editing.cpp`: scale становится `float`, применяется через `m_fScale`.
- Миграция JSON: при десериализации старого `entity.scale` (массив) — для объектных сущностей передавать `scale[0]` в `Object.scale`; для прочих — игнорировать.
- Обновить Lua декларации (`../ingame/definitions/core`): убрать `scale` у `Entity`, добавить `scale` у `ObjectComponent`.

## Capabilities

### New Capabilities

- `object-scale`: per-object uniform scale интегрированный через нативный `CObject::m_fScale`, с сохранением/загрузкой и миграцией со старого формата.

### Modified Capabilities

(нет — `entity-orientation` не трогается, scale никогда не был частью orientation-спеки)

## Impact

**Код** (правка):
- `src/data/entity.h/cpp`
- `src/data/components/object.h/cpp`
- `src/data/components/actor.cpp`
- `src/data/components/vehicle.cpp`
- `src/data/components/pickup.cpp`
- `src/data/components/particle.cpp`
- `src/data/components/firework.cpp`
- `src/data/components/checkpoint.cpp`
- `src/data/components/train.cpp`
- `src/data/components/object/object_move_by_path.cpp`
- `src/data/components/object/object_trigger_move.cpp`
- `src/rendering/in_game/object_transform_editing.cpp`

**Код** (удаление):
- `src/utils/matrix_utils.h`

**Внешнее**:
- Lua декларации: `../ingame/definitions/core/Entity.lua`, `ObjectComponent.lua` (или аналоги)
- Локализация: ключ(и) для поля scale в Object (`../ingame/languages/en.json`)

**Совместимость**:
- Существующие проекты с `entity.scale != [1,1,1]` — для объектов подхватится `scale[0]`; для прочих типов scale из сохранения молча отбрасывается (ожидаемое поведение, т.к. он и так был сломан).

**Lua API (BREAKING)**:
- `entity.scale` больше не существует. Использование → `objectComponent.scale`.

**Риски**:
- Если в Lua-скриптах пользовательских аддонов есть обращение к `entity.scale`, они сломаются при загрузке. Нужно пробежаться по `../ingame/addons/core` и проверить.
