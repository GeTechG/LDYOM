## 1. Object component: добавить scale-поле и применение

- [x] 1.1 В `src/data/components/object.h` расширить `DirtyFlags` значением `Scale`
- [x] 1.2 Добавить `float scale = 1.0f;` в класс `Object`
- [x] 1.3 В `Object::to_json` сериализовать `j["scale"] = scale`
- [x] 1.4 В `Object::from_json` читать `scale` с fallback на `1.0f`, если ключ отсутствует
- [x] 1.5 В `Object::editorRender` добавить `ImGui::DragFloat("##scale", &scale, …)` без clamp — помечать `dirty |= Scale` при изменении. Добавить лейбл из локализации (ключ `scale`).
- [x] 1.6 В `Object::spawn` после создания/привязки `this->handle` — записать `this->handle->m_fScale = this->scale`
- [x] 1.7 В `Object::onUpdate` обработать `dirty & Scale` — записать `handle->m_fScale = scale`
- [x] 1.8 В `Object::despawn` перед возвратом в пул — записать `handle->m_fScale = 1.0f` (если handle валиден)
- [x] 1.9 Обновить `Object::sol_lua_register` — добавить биндинг `scale` в usertype

## 2. Entity: удалить scale и scale-колбэки

- [x] 2.1 Удалить `std::array<float, 3> scale = {1.0f, 1.0f, 1.0f};` из `Entity`
- [x] 2.2 Удалить `getScaleCallback` / `setScaleCallback` поля
- [x] 2.3 Изменить сигнатуры `setGetTransformCallbacks` / `setSetTransformCallbacks` — убрать параметр scale
- [x] 2.4 Удалить scale из `clearGetTransformCallbacks` / `clearSetTransformCallbacks`
- [x] 2.5 Удалить scale из `Entity::to_json` (новый формат — без ключа `scale` в Entity)
- [x] 2.6 Обновить `Entity::sol_lua_register` — удалить поле `scale`, если было зарегистрировано
- [x] 2.7 Обновить `updateSetTransformCallbacks` под новую сигнатуру

## 3. Миграция legacy JSON

- [x] 3.1 Добавить `std::optional<float> _legacyScale;` (приватное поле `Entity`, не сериализуется, комментарий «migration Phase 1»)
- [x] 3.2 В `Entity::from_json` при наличии `"scale"` в json — прочитать `scale[0]` в `_legacyScale`
- [x] 3.3 В `Scene` / месте парсинга компонентов entity (скорее всего `Scene::from_json` или `Entity::from_json` после парсинга компонентов) — если `_legacyScale.has_value()` и есть компонент `Object`, скопировать значение в `Object::scale`, затем `_legacyScale.reset()`
- [x] 3.4 Добавить `spdlog::debug` лог миграции: `"Migrated legacy entity.scale={} to Object.scale for entity '{}'"`

## 4. Апдейт компонентов, использующих старую scale-API

- [x] 4.1 `src/data/components/actor.cpp` — убрать scale-лямбду из `setSetTransformCallbacks`, обновить `setGetTransformCallbacks`
- [x] 4.2 `src/data/components/vehicle.cpp` — то же
- [x] 4.3 `src/data/components/pickup.cpp` — то же, удалить вызов `scaleMatrix(*CPickups::aPickUps[...].m_pObject->m_matrix, scale)`
- [x] 4.4 `src/data/components/particle.cpp` — то же, удалить вызов `scaleMatrix(matrix, scale)`
- [x] 4.5 `src/data/components/firework.cpp` — удалить закомментированный scale-коллбэк
- [x] 4.6 `src/data/components/checkpoint.cpp` — удалить закомментированный scale-коллбэк
- [x] 4.7 `src/data/components/train.cpp` — проверить (grep показал совпадение) и убрать если есть
- [x] 4.8 `src/data/components/object.cpp` — убрать scale-лямбду из старого `setSetTransformCallbacks` (scale теперь через `m_fScale`)

## 5. Object-related utilities

- [x] 5.1 `src/data/components/object/object_move_by_path.cpp` — заменить `this->entity->scale` на чтение `Object::scale` через `entity->getComponent(Object::TYPE)`; убрать вызовы `scaleMatrix` (теперь `m_fScale` делает работу)
- [x] 5.2 `src/data/components/object/object_trigger_move.cpp` — то же
- [x] 5.3 `src/rendering/in_game/object_transform_editing.cpp` — `m_scale` меняется на `float`; в `openTransformEditing` принимать `float` вместо `std::array<float,3>`; применение через `m_object->m_fScale = m_scale` вместо `scaleMatrix`
- [x] 5.4 Обновить всех вызывающих `ObjectTransformEditing::openTransformEditing` под новую сигнатуру
- [x] 5.5 `src/rendering/in_game/object_transform_editing.cpp` UI: `DragFloat` scale без clamp, без three-axis input

## 6. Удаление мёртвого кода

- [x] 6.1 Удалить файл `src/utils/matrix_utils.h`
- [x] 6.2 Удалить все `#include "matrix_utils.h"` и `#include <matrix_utils.h>` — grep и чистка
- [x] 6.3 Убедиться grep-ом, что `scaleMatrix(` больше нигде не встречается в `src/`

## 7. Lua декларации и локализация

- [x] 7.1 Обновить декларацию `Entity` в `../ingame/definitions/core` — удалить поле `scale`
- [x] 7.2 Обновить/создать декларацию `ObjectComponent` — добавить `scale: number`
- [x] 7.3 Добавить ключ `scale` в `../ingame/languages/en.json` для UI-лейбла Object-компонента (если ещё не существует)
- [x] 7.4 Добавить/обновить ключ в `ru.json` (если есть), либо оставить fallback на английский

## 8. Проверка аддонов пользователя

- [x] 8.1 `grep -r "\.scale" ../ingame/addons/core` — найти упоминания `.scale` у entity
- [x] 8.2 Переписать найденные обращения: `entity.scale[0]` → `entity:getComponent("object").scale` (или аналог согласно существующему Lua-API)

## 9. Валидация

- [x] 9.1 Собрать Debug (`cmake --build build/Debug`)
- [x] 9.2 Вручную: создать новый проект, добавить entity с компонентом Object, задать scale=2.5 → визуально объект крупнее
- [x] 9.3 Вручную: задать scale=0.5 → объект мельче
- [x] 9.4 Вручную: задать scale=-1 → проверить поведение (должно принять без ошибки; визуальный эффект — по усмотрению движка, не падать)
- [x] 9.5 Вручную: сохранить проект, перезагрузить → scale сохраняется
- [x] 9.6 Вручную: открыть старый проект (с `entity.scale: [2.0, 2.0, 2.0]` и компонентом Object) → после загрузки Object.scale == 2.0; пересохранение уже в новом формате
- [x] 9.7 Вручную: entity с только `Actor` — нет UI для scale в редакторе
- [x] 9.8 Вручную: Lua-скрипт `print(objectComponent.scale)` — выводит значение; `entity.scale` — `nil`
- [x] 9.9 Запустить `openspec validate move-entity-scale-to-object` — без ошибок
