# MiniRT
Ray tracing projet

## TODO

1. t_color: double or int?
2. t_object: linked list?

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

#### func `hit_sphere()`

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

#### func `sphere_normal_at`

给定球体上一个点 p，计算该点的法线向量。获取球体(sphere)的法线(normal)在(at)[某个点]

法线向量(Normal)是一个垂直于表面并指向外部的单位向量。它在光照计算中至关重要，因为它决定了光线如何从表面反射。

对于球体上的任意一点 p，它的法线向量就是从球心 C 指向该点 p 的向量。通过向量减法可得：

Normal = p - C

计算机图形学中，法线向量常被要求是单位向量，这能极大简化后续运算。


## Structs

### `t_camera`

场景的观察者。在渲染前被设置。

- `t_vector viewpoint`: 摄像机的位置，是所有初始光线的共同出发点
- `t_vector direction`: 摄像机的朝向。不是某条具体光线的方向，而是整个视野的中心轴线方向。即摄像机“正对着”哪里。
- `int fov`: 视场角。摄像机独有的属性。它定义了视野的宽度。

**任务**： 根据屏幕上每一个像素的位置，结合自身的属性，计算出一条应该穿过该像素的光线t_ray。

### `t_ray`

数学概念，即一条射线。

- `t_vector origin`: 光线的起点。
- `t_vector direction`: 光线的方向。


