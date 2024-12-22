; ModuleID = 'nested_struct.c'
source_filename = "nested_struct.c"
target datalayout = "..."
target triple = "..."

; 定义结构体类型
%struct.Point = type { i32, i32 }
%struct.Rectangle = type { %struct.Point, %struct.Point }

define i32 @main() {
entry:
  ; 分配 rect 在栈上的空间
  %rect = alloca %struct.Rectangle, align 4

  ; 访问 rect.topLeft.x 的 GEP：
  ; %struct.Rectangle 有两个成员：[0] topLeft (type %struct.Point), [1] bottomRight (type %struct.Point)
  ; %struct.Point 有两个成员：[0] x (int), [1] y (int)
  ;
  ; GEP 参数解释：
  ; getelementptr inbounds %struct.Rectangle, %struct.Rectangle* %rect, i32 0, i32 0, i32 0
  ; 从 %rect 开始，进入第0个成员（topLeft），再在 topLeft 内取第0个成员（x）。
  
  %topLeft_x_ptr = getelementptr inbounds %struct.Rectangle, %struct.Rectangle* %rect, i32 0, i32 0, i32 0
  store i32 1, i32* %topLeft_x_ptr, align 4

  ; rect.topLeft.y
  %topLeft_y_ptr = getelementptr inbounds %struct.Rectangle, %struct.Rectangle* %rect, i32 0, i32 0, i32 1
  store i32 2, i32* %topLeft_y_ptr, align 4

  ; rect.bottomRight.x
  %bottomRight_x_ptr = getelementptr inbounds %struct.Rectangle, %struct.Rectangle* %rect, i32 0, i32 1, i32 0
  store i32 10, i32* %bottomRight_x_ptr, align 4

  ; rect.bottomRight.y
  %bottomRight_y_ptr = getelementptr inbounds %struct.Rectangle, %struct.Rectangle* %rect, i32 0, i32 1, i32 1
  store i32 20, i32* %bottomRight_y_ptr, align 4

  ; 计算 width = rect.bottomRight.x - rect.topLeft.x
  %brx_val = load i32, i32* %bottomRight_x_ptr, align 4
  %tlx_val = load i32, i32* %topLeft_x_ptr, align 4
  %width = sub i32 %brx_val, %tlx_val

  ; 计算 height = rect.bottomRight.y - rect.topLeft.y
  %bry_val = load i32, i32* %bottomRight_y_ptr, align 4
  %tly_val = load i32, i32* %topLeft_y_ptr, align 4
  %height = sub i32 %bry_val, %tly_val

  ; return width * height
  %area = mul i32 %width, %height
  ret i32 %area
}
