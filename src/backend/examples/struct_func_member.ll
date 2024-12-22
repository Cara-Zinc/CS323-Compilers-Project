; 定义结构体类型
%struct.Point = type { i32, i32, %struct.Point (%struct.Point*)* }

; 定义reverse函数
define %struct.Point @Point_reverse(%struct.Point* %p) {
entry:
  ; 获取 p->x 和 p->y
  %x_ptr = getelementptr inbounds %struct.Point, %struct.Point* %p, i32 0, i32 0
  %y_ptr = getelementptr inbounds %struct.Point, %struct.Point* %p, i32 0, i32 1
  %x = load i32, i32* %x_ptr
  %y = load i32, i32* %y_ptr

  ; 计算反转后的坐标 (-x, -y)
  %negx = sub i32 0, %x
  %negy = sub i32 0, %y

  ; 创建一个临时的 Point 用于返回
  %tmp = alloca %struct.Point
  %tmp_x = getelementptr inbounds %struct.Point, %struct.Point* %tmp, i32 0, i32 0
  %tmp_y = getelementptr inbounds %struct.Point, %struct.Point* %tmp, i32 0, i32 1
  store i32 %negx, i32* %tmp_x
  store i32 %negy, i32* %tmp_y

  ; 对于函数指针字段，目前可以先不赋值，也可以在你的语言语义中考虑返回的 Point 是否需要复制其函数指针字段。
  ; 假设我们也给返回的点赋同样的函数指针:
  %tmp_reverse_ptr = getelementptr inbounds %struct.Point, %struct.Point* %tmp, i32 0, i32 2
  store %struct.Point (%struct.Point*)* @Point_reverse, %struct.Point (%struct.Point*)** %tmp_reverse_ptr

  %res = load %struct.Point, %struct.Point* %tmp
  ret %struct.Point %res
}


define i32 @main() {
entry:
  %p = alloca %struct.Point
  ; p.x = 10
  %px = getelementptr inbounds %struct.Point, %struct.Point* %p, i32 0, i32 0
  store i32 10, i32* %px

  ; p.y = 20
  %py = getelementptr inbounds %struct.Point, %struct.Point* %p, i32 0, i32 1
  store i32 20, i32* %py

  ; p.reverse = @Point_reverse
  %pr = getelementptr inbounds %struct.Point, %struct.Point* %p, i32 0, i32 2
  store %struct.Point (%struct.Point*)* @Point_reverse, %struct.Point (%struct.Point*)** %pr

  ; 调用 p.reverse(&p)，得到反转后的点
  %fptr = load %struct.Point (%struct.Point*)*, %struct.Point (%struct.Point*)** %pr
  %res = call %struct.Point %fptr(%struct.Point* %p)

  ; 从返回的点中取出x和y
  %res_x_ptr = getelementptr inbounds %struct.Point, %struct.Point* %alloca_for_res, i32 0, i32 0
  %res_y_ptr = getelementptr inbounds %struct.Point, %struct.Point* %alloca_for_res, i32 0, i32 1
  ; 注意这里%alloca_for_res是指存res的内存，需要先有一段内存来存%res的值。如下：
  %alloca_for_res = alloca %struct.Point
  store %struct.Point %res, %struct.Point* %alloca_for_res
  %res_x = load i32, i32* %res_x_ptr
  %res_y = load i32, i32* %res_y_ptr

  ; 假设最终返回反转后的x+y作为main返回值
  %sum = add i32 %res_x, %res_y
  ret i32 %sum
}
