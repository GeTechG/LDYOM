## ADDED Requirements

### Requirement: Object component owns uniform scale

Компонент `Object` SHALL хранить поле `scale` типа `float` со значением по умолчанию `1.0f`. Scale является единственным каноничным местом хранения масштаба для объектных сущностей. `Entity` MUST NOT содержать поле `scale` и связанные transform-колбэки для scale.

#### Scenario: Default scale when object is created

- **WHEN** создаётся новый компонент `Object`
- **THEN** его `scale` равен `1.0f`
- **AND** у `Entity` отсутствует поле `scale`

#### Scenario: Scale is not clamped

- **WHEN** пользователь или скрипт задаёт `scale` произвольным `float` (включая ноль, отрицательные значения и большие величины)
- **THEN** система SHALL принять значение без модификации
- **AND** значение SHALL сохраниться в JSON при сохранении проекта

### Requirement: Scale applied via native CObject::m_fScale

Компонент `Object` SHALL применять scale исключительно через запись в `CObject::m_fScale` заспавненной сущности. Компонент MUST NOT умножать столбцы матрицы (`matrix.right/up/at *= ...`) напрямую. Визуальный рендер масштабированной модели выполняется игровым движком в `CObject::PreRender` через `RwMatrixScale`.

#### Scenario: Scale applied on spawn

- **WHEN** компонент `Object` спавнит свою модель в игре
- **THEN** `CObject::m_fScale` немедленно устанавливается равным `Object.scale`
- **AND** прямых вызовов `scaleMatrix` или ручного умножения матрицы не производится

#### Scenario: Scale changed at runtime

- **WHEN** `Object.scale` изменяется в редакторе или из Lua у уже заспавненного объекта
- **THEN** `CObject::m_fScale` SHALL быть обновлён на новое значение в ближайшем `onUpdate`
- **AND** движок GTA на следующем `PreRender` отрендерит объект с новым масштабом

#### Scenario: Scale resets on despawn

- **WHEN** объект деспавнится (`onReset` / `despawn`)
- **THEN** перед возвратом `CObject` в пул `m_fScale` SHALL быть сброшен в `1.0f`, чтобы переиспользующий этот `CObject` код не унаследовал нестандартный масштаб

### Requirement: Scale UI exposed only on Object component

UI для редактирования scale SHALL рендериться только в `editorRender` компонента `Object`. Компоненты `Actor`, `Vehicle`, `Pickup`, `Particle`, `Firework`, `Checkpoint`, `Train` MUST NOT содержать UI для масштаба. Глобальная секция transform в Entity-редакторе MUST NOT показывать поле scale.

#### Scenario: Object editor shows scale input

- **WHEN** пользователь открывает редактор entity, содержащей компонент `Object`
- **THEN** в секции компонента `Object` отображается числовое поле ввода `scale` без верхних/нижних ограничений

#### Scenario: Non-object entities do not show scale

- **WHEN** пользователь открывает редактор entity, содержащей компонент `Actor` (или `Vehicle`, `Pickup`, `Particle`, `Firework`, `Checkpoint`, `Train`), но без `Object`
- **THEN** поле scale отсутствует в интерфейсе редактора

### Requirement: JSON migration from legacy Entity.scale

Сериализация `Object.scale` SHALL сохраняться внутри JSON-блока компонента `Object` под ключом `"scale"`. При десериализации проектов старого формата, где `"scale"` (массив из 3 float) хранится в блоке `Entity`, система SHALL выполнить миграцию: если у сущности есть компонент `Object`, то `Object.scale` инициализируется значением `legacy_entity_scale[0]`. Для всех прочих типов сущностей устаревшее `Entity.scale` MUST быть молча отброшено.

#### Scenario: Legacy object entity is migrated

- **WHEN** загружается проект, где entity имеет `"scale": [2.5, 2.5, 2.5]` на уровне entity и содержит компонент `Object`
- **THEN** после загрузки `Object.scale == 2.5f`
- **AND** поле `scale` у `Entity` отсутствует в объекте в памяти и при последующей сериализации

#### Scenario: Legacy non-object entity drops scale silently

- **WHEN** загружается проект, где entity имеет `"scale": [1.8, 1.8, 1.8]` и НЕ содержит компонент `Object` (например, только `Actor`)
- **THEN** устаревшее поле игнорируется без ошибки
- **AND** при последующей сериализации `"scale"` в entity-блоке отсутствует

#### Scenario: Non-uniform legacy scale takes first axis

- **WHEN** загружается проект, где `"scale": [3.0, 1.0, 1.0]` (не-uniform) у entity с компонентом `Object`
- **THEN** `Object.scale == 3.0f` (берётся `scale[0]`)
- **AND** loss остальных компонент остаётся без предупреждения (устаревший формат всё равно был некорректен)

### Requirement: Lua API exposes scale on ObjectComponent only

Lua биндинги SHALL предоставлять поле `scale : number` у типа `ObjectComponent` (через sol2 usertype). Тип `Entity` MUST NOT иметь биндинга `scale`. Декларативный Lua-файл `../ingame/definitions/core` SHALL отражать это: `Entity.lua` без поля scale, декларация для `ObjectComponent` содержит `scale: number`.

#### Scenario: Reading scale from Lua

- **WHEN** Lua-скрипт вызывает `objectComponent.scale` у компонента `Object`
- **THEN** возвращается текущее значение `float`

#### Scenario: Writing scale from Lua

- **WHEN** Lua-скрипт присваивает `objectComponent.scale = 2.0`
- **THEN** значение сохраняется и при следующем `onUpdate` применяется к `CObject::m_fScale`

#### Scenario: Entity.scale no longer exists in Lua

- **WHEN** Lua-скрипт обращается к `entity.scale`
- **THEN** возвращается `nil` (поле удалено из usertype)
