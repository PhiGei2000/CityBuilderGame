# Components

## Base Component

The base class of all components is the "Component" class.

## Table of components
- `BuildingComponent`
- `CameraComponent`
- `CarComponent`
- `DebugComponent`
- `EnvironmentComponent`
- `LightComponent`
- `MeshComponent`
- `ParkingComponent`
- `RoadComponent`
- `TransformationComponent`
- `VelocityComponent`

## Details
### Building Component
---
The Building Component is an indicator that the entity is a building.

#### Properies
---

|Property| Type | Description |
|---| --- | --- |
| `type` |`BuildingType` | Type of the building |
| `gridPosition`|`glm::ivec2` | Position of the building |
| `rotation`|`int` | Roation of the building in terms of $\frac{\pi}{2}$ |
| `size`|`glm::ivec2` | The occupied space |
| `preview`|`bool` | If `true` the preview of the building will be rendered


### Camera Component
---
The Camera Component is only assigned to the camera entity and holds data about the orientation, size and range of the view. In addition to this the camera matrices are calculated and stored in this component

#### Properies and Functions

| Property | Type | Description |
| --- | --- | --- |
| `up`, `front`, `right` | `glm::vec3` | The orthonomal basis of the camera space in world coordinates |
| `width`, `height` | `float` | The width and the height of the camera in pixels. These values are updated by the `glFramebufferSizeCallback`. |
| `fov` | `float` | The field of view |
| `near`, `far` | `float` | The range in which vertices were rendered |
| `yaw`, `pitch` | `float` | The angels that describe the cameras orientation |
| `projectionMatrix` | `glm::mat4` | The matrix that transformes the view space into the clip space |
| `viewMatrix` |  `glm::mat4` | The matrix that transformes the world space into the view space |

| Function | Description |
| --- | --- |
| `void calculateMatrices(const TransformationComponent& transform)` | Updates the view and projection matrix with the given transformation component of the camera
| `void calculateVectors()` | Updates the `front`, `right` and `up` vector of the camera using the `yaw` and `pitch` values

### Car Component
---
The car component is assigned to entities that use roads

#### Properies

| Property | Type | Description |
| --- | --- | --- |
| `driving` | `bool` | If `true` the car has a destination and is moving |
| `lastPathPosition` | `glm::vec3` | The position of the path node which the car has passed |
| `currentPath` | `glm::vec3` | The positions of the nodes of the path that the car will follow