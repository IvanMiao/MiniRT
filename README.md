# MiniRT
Ray tracing projet

## TODO

1. parsing
	- code in src/parsing/
	- add tools in src/utils/
	- .rt files for test
2. bonus: shader - specular reflection [shininess -> calculate spec_light(all_lights.c)]
3. bonus: anti-aliasing

## Architecture


## Phases

1. parsing
2. init mlx, init events
3. setup scene
4. Main loop: Calculate ray (camera -> ray)
5. Main loop: Calculate hit points
6. Main loop: Calculate lighting (ambiant color + diffuse color)
7. Main loop: put pixel

## Math

### Vector

- 向量减法 `vector_sub(A, B)`: 得到 B 的终点指向 A 的终点的新向量 (A - B)。
- 向量点积 `vector_dot(A, B)`: 两个向量的乘积，结果是一个标量。它衡量两个向量方向的相似程度。点积的几何意义：`A · B = |A| * |B| * cos(theta)`。
	- 如果两个向量垂直，它们的点积为 0
	- 如果两个向量为单位向量，点积就是它们夹角的余弦值。
	- 向量 A 与自身的点积等于其长度的平方：|A|^2 = A · A。
- 向量归一化 `vector_normalize(A)`: 将一个向量的长度变为 1，但保持其方向不变，即把这个向量单位化。计算方法： v / |v|, 单位向量在光线追踪中非常常用，例如表示光线方向或表面法线。

### Ray

一条光线可以用一个起点 (Origin) 和一个方向 (Direction) 来精确描述。光线上任意一点 P 的位置，可以用一个参数 t 来表示。

`P(t) = O + t * D`

- `P(t)`: 光线上距离起点 t 个单位长度的点的位置向量
- `O`: 光线的起点 origin，一个位置向量
- `D`: 光线的方向 direction，一个单位向量
- `t`: 一个标量，表示从起点 O 沿着方向 D 移动的"距离"或"时间"。当 t>0 时， 点 P(t) 在光线的正前方。t=0, P(0) = 0，即光线起点。


### Sphere

一个球体可以用一个球心 (Center) 和一个半径 (Radius) 来定义。球体表面的所有点 P 都满足一个条件：该点到球心的距离正好等于半径。

`| P - C |^2 = r^2` 或 `| P - C | = r`

- `P`: 球体表面上任意一点
- `C`: 球心的位置向量
- `r`: 球体半径

利用向量点积的特性， v · v = |v|^2, 球体公式可以写成：
`(P - C) · (P - C) = r^2`

#### sphere hit func `hit_sphere()`

判断一条给定的光线是否与一个给定的球体相交。若相交，找出最近的交点。

数学推导：
1. 若存在光线和球体的交点 P，则点P同时满足光线方程和球体方程。
	- P = O + t * D
	- (P - C) · （P - C） = r^2

2. 将光线方程代入球体方程： `((O + t*D) - C) · ((O + t*D) - C) = r^2`

3. 简化方程，令oc = O - C， oc 为球心C指向光线起点O的向量。方程变为： `(oc + t*D) · (oc + t*D) = r^2`

4. 利用点积的分配率展开左边： `(oc · oc) + 2 * (oc · (t*D)) + ((t*D) · (t*D)) = r^2`

5. 由于t是标量，可以从中提取出来： `(oc · oc) + 2t * (oc · D) + t^2 * (D · D) = r^2`

6. 将 r^2 移到左边，可以整理成一元二次方程： `(D · D) * t^2 + (2 * (oc · D)) * t + (oc · oc - r^2) = 0`

7. 这个一元二次方程的三个系数A， B， C 分别为：
	- A = D · D
	- B = 2 * (oc · D)
	- C = oc · oc - r^2

8. 利用根的判别式即可求解这个方程，得到光线与球体相交时的t值： `t = (-B ± sqrt(B^2 - 4AC)) / (2A)`

参数：
- `oc = vector_sub(ray->origin, sp->center)`: 球心到光线起点的向量 O - C
- `a = vector_dot(ray->direction, ray->direction)`： A = D · D
- `b = 2.0 * vector_dot(oc, ray->direction)` ： B = 2 * (oc · D)
- `c = vector_dot(oc, oc) - sp->radius * sp->radius`： C = oc · oc - r^2
- `discriment = b * b - 4 * a * c`

#### sphere normal func `sphere_normal_at`

给定球体上一个点 p，计算该点的法线向量。获取球体(sphere)的法线(normal)在(at)[某个点]

法线向量(Normal)是一个垂直于表面并指向外部的单位向量。它在光照计算中至关重要，因为它决定了光线如何从表面反射。

对于球体上的任意一点 p，它的法线向量就是从球心 C 指向该点 p 的向量。通过向量减法可得：

Normal = p - C

计算机图形学中，法线向量常被要求是单位向量，这能极大简化后续运算。


### Cylinder

一个有限高度的圆柱体，在3D空间中可以由以下几个参数定义：

- 基底圆心 (Center): 我们通常将其定义为底部圆盘的中心点，记作 C。
- 轴向向量 (Normal/Axis): 一个从基底圆心 C 指向顶部圆心的单位向量，记作 V。这个向量定义了圆柱体的朝向和中心轴。
- 半径 (Radius): 圆柱体的半径，记作 r。
- 高度 (Height): 圆柱体的高度，记作 h。

圆柱体的侧面是所有到中心轴的垂直距离等于半径 r 的点的集合。

想象空间中任意一点 P。我们想知道它是否在无限延伸的圆柱体侧面上。

1.  从圆柱体基底圆心 `C` 到点 `P` 有一个向量 `oc = P - C`。
2.  这个向量 `oc` 可以被分解为两个相互垂直的分量：
    *   一个**平行于**轴向 `V` 的分量。
    *   一个**垂直于**轴向 `V` 的分量。
3.  这个垂直分量的长度，就是点 `P` 到中心轴的垂直距离。
4.  利用向量投影，`oc` 在轴向 `V` 上的投影向量是 `(oc · V) * V`。
5.  根据向量减法，从 `oc` 中减去它的平行分量，剩下的就是垂直分量：`oc_perp = oc - (oc · V) * V`。
6.  点 `P` 在圆柱体侧面上的条件是，这个垂直分量的长度等于半径 `r`，即 `|oc_perp| = r`。
7.  为了避免开方，我们使用平方形式：`|oc_perp|^2 = r^2`。 即 `|oc - (oc · V) * V|^2 = r^2`

根据向量的勾股定理 `|a|^2 = |a_parallel|^2 + |a_perp|^2`，可以把上式简化为：

`|oc_perp|^2 = |oc|^2 - |oc_parallel|^2`

`|oc_perp|^2 = (oc · oc) - ((oc · V) * V · (oc · V) * V)`

`V` 是单位向量，`V · V = 1`，因此：

`|oc_perp|^2 = (oc · oc) - (oc · V)^2`

所以，无限圆柱体侧面的方程为：**(P - C) · (P - C) - ((P - C) · V)^2 = r^2**

#### cylinder hit func `hit_cylinder`

1. 计算光线与**无限圆柱体侧面**的交点。
2. 计算光线与**两个顶/底盖**的交点。
3. 在所有有效的交点中，找出离光线起点最近的那个。

将光线方程 `P(t) = O + t*D` 代入上面推导出的无限圆柱体方程，可得：

 `P - C = (O + t*D) - C`

 令光线起点到圆柱体基底中心的向量为 oc_ray,则 `oc_ray = O - C`

 代入方程：

 `((oc_ray + t*D) · (oc_ray + t*D)) - ((oc_ray + t*D) · V)^2 - r^2 = 0`

 又是一个关于 `t` 的一元二次方程。三个系数分别为：

 - A： `A = (D · D) - (D · V)^2`
 - B： `B = 2 * ((D · oc_ray) - (D · V) * (oc_ray · V))`
 - C： `C = (oc_ray · oc_ray) - (oc_ray · V)^2 - r^2`

 
#### cylinder normal func `cylinder_normal_at`




### func `setup_cam_coords`

相机的定位、定向和镜头选择

**问题**：建立虚拟摄像机 (The Virtual Camera) —— 如何从一个数学点（摄像机位置）向一个二维像素网格（屏幕）发射精确的光线。

该函数为虚拟摄像机建立一个**局部坐标系**，并定义好它的视锥体 (View Frustum)。

建立一个以摄像机为中心的坐标系。在这个坐标系里，我们永远知道摄像机的右边、上边、后边(u, v, w)这三个摄像机局部坐标系的基向量。

1. 设置宽高比
2. 定义“世界”的上方 world_up，假设为Y轴正方面，用来确定摄像机的右和上方
3. 计算摄像机的后方 w: 计算机图形学约定摄像机朝其局部坐标系的-Z轴“看”，因此其后方为摄像机朝向方向的取反
4. 计算摄像机的右方 u: 向量叉乘 cross product，根据右手定则， (世界向上) × (摄像机后方) 得到摄像机右方向量，它同时垂直于world_up 和 w
5. 计算摄像机的右方 v: 再用cross product, (后方) × (右方) 就会得到指向摄像机“真正上方”的向量v。
6. FOV为摄像机的“镜头”，决定视野宽窄。假设在camera前有一个虚拟的成像平面，通过三角函数，tan(半个FOV角度) = (半个成像平面的高度) / (到平面的距离)。假设成像平面离camera距离为1,所以half_height就直接等于 tan(theta / 2.0)。
7. 根据宽高比算出宽度 half_width。


### func `gen_cam_ray`

为屏幕上的每一个像素 (x, y) 发射一条独一无二的光线。这条光线从摄像机原点出发，穿过这个像素在虚拟成像平面上对应的那个点。


```c
screen_x = (2.0 * (x + 0.5) / WIDTH - 1.0) * camera->half_width;
screen_y = -(2.0 * (y + 0.5) / HEIGHT - 1.0) * camera->half_height;
```
1. 将像素坐标 (x, y)映射到虚拟成像平面坐标
	- 像素坐标 x 的范围是 [0, WIDTH-1]。
	- (x + 0.5) 是为了让光线穿过像素中心，而不是左上角。
	- (x + 0.5) / WIDTH 将其归一化到 [0, 1] 范围。
	- 2.0 * ... - 1.0 将其映射到 [-1, 1] 的标准范围。
	- 乘以 half_width/half_height 将其缩放到之前计算的(由FOV确定的)虚拟成像平面的实际大小。
	- screen_y 前的负号是因为屏幕坐标y通常向下为正，而我们的3D世界坐标y向上为正。

```c
t_vector	screen_point;

screen_point = camera->viewpoint;
screen_point = vector_add(screen_point, vector_mult(camera->u, screen_x));
screen_point = vector_add(screen_point, vector_mult(camera->v, screen_y));
screen_point = vector_add(screen_point, vector_mult(camera->w, -1.0));
```

2. 计算该点在世界坐标系中的确切位置 (screen_point)：
	- 从摄像机的位置 (viewpoint) 开始。
	- 沿着摄像机的“右方”(u) 移动 screen_x 个单位。
	- 再沿着摄像机的“上方”(v) 移动 screen_y 个单位。
	- 最后，沿着摄像机的“前方”移动1个单位（即沿着“后方”w移动-1个单位）。这样，我们就定位到了虚拟成像平面上那个精确的点。


```c
ray.origin = camera->viewpoint;
ray.direction = vector_sub(screen_point, camera->viewpoint);
ray.direction = vector_normalize(ray.direction);
```

3. 生成最终的光线：
	- 光线的起点 (origin) 就是摄像机的位置。
	- 光线的方向 (direction) 就是从起点指向我们刚刚计算的 screen_point 的向量。
	- 最后将方向向量单位化。

### func `is_in_shadow`

计算阴影：判断一个点是否处于阴影之中。

从物体表面的某一点 `hit_point` 向光源 `light_pos` 发射一条新的光线（称为**阴影光线shadow ray**）。如果这条光线在到达光源之前，撞到了场景中的**任何其他物体**，那么 `hit_point` 就被遮挡了，处于阴影中。

1. 构建一条从物体表面指向光源的“阴影光线”，并设定一个最大有效距离（到光源的距离）
2. 遍历场景中的所有物体
3. 每一个物体，调用 `hit_for_shadow` 
4. `hit_for_shadow` 根据物体的具体类型，调用正确的数学函数来计算阴影光线是否与该物体相交
5. `is_in_shadow` 检查返回的相交距离 `t`，如果发现一个有效的遮挡物（`0 < t < dist_to_light`），就判定该点在阴影中


### func `combine_light`



## Structs

### `t_camera`

场景的观察者。在渲染前被设置。

- `t_vector viewpoint`: 摄像机的位置，是所有初始光线的共同出发点
- `t_vector direction`: 摄像机的朝向。不是某条具体光线的方向，而是整个视野的中心轴线方向。即摄像机“正对着”哪里。
- `int fov`: 视场角。摄像机独有的属性。它定义了视野的宽度。
- `u`: 规定摄像机的右边方向
- `v`: 规定摄像机的上边方向
- `w`: 规定摄像机的后边方向，以上三个相互垂直的单位向量(u, v ,w)构成摄像机的局部坐标系基向量


**任务**： 根据屏幕上每一个像素的位置，结合自身的属性，计算出一条从camera原点射出、穿过该像素的光线t_ray。

### `t_ray`

数学概念，即一条射线。

- `t_vector origin`: 光线的起点。
- `t_vector direction`: 光线的方向。


### `t_cylinder`

定义圆柱体的结构体。

- `t_vector center`: 中心点
- `t_vector normal`: 轴线方向
- `double	diameter`: 直径
- `double	height`: 高度
- `t_color	color`: 颜色

