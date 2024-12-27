# LLVM IR Generation

## 项目结构
```
.
├──defs
│
│
│
│
└──
```


- - -
# LLVM IR 基础语法与示例

LLVM IR (Intermediate Representation) 是 LLVM 编译器框架的核心中间语言。它是一种低级、类似汇编的编程语言，同时支持高级语言特性。LLVM IR 的设计目标是易于分析和优化，并与具体的硬件平台无关。

## LLVM IR 的核心特点
- **三种表示形式**：
  1. **文本形式 (.ll)**：可读性强，便于调试。
  2. **二进制形式 (.bc)**：高效存储，便于分发。
  3. **内存形式**：LLVM 内部使用。
- **严格的类型系统**：所有操作数和指令都需要显式类型。
- **单静态赋值 (SSA) 形式**：每个变量在定义后不可改变，有助于编译器优化。

---

## 基础语法

### 模块结构
一个 LLVM IR 文件通常由以下几部分组成：

1. **Target 信息**
   ```llvm
   target datalayout = "..."
   target triple = "..."
   ```
   描述目标平台的内存布局和架构。

2. **全局声明**
   包括全局变量、函数声明、常量等。

3. **函数定义**
   描述函数体内的控制流和操作。

### 基础类型
- `i1`：1 位布尔值。
- `i8`：8 位整数。
- `i32`：32 位整数。
- `float` / `double`：单精度/双精度浮点数。
- `void`：无返回值。
- `[n x <type>]`：n 个 `<type>` 类型的数组。

### 核心指令
1. **变量分配**：
   ```llvm
   %a = alloca i32, align 4 ; 在栈中分配一个 i32 类型变量
   ```

2. **存储与加载**：
   ```llvm
   store i32 42, i32* %a    ; 将 42 存储到变量 %a 中
   %b = load i32, i32* %a   ; 从变量 %a 中加载值到 %b
   ```

3. **算术操作**：
   ```llvm
   %sum = add i32 %x, %y    ; 计算 %x + %y
   ```

4. **条件跳转**：
   ```llvm
   %cond = icmp eq i32 %x, 0   ; 比较 %x 是否等于 0
   br i1 %cond, label %if, label %else ; 条件跳转
   ```

5. **函数调用**：
   ```llvm
   %result = call i32 @add(i32 %a, i32 %b) ; 调用函数 add
   ```

---

## 简单示例

### 1. Hello World 示例
使用 LLVM IR 实现一个打印 "Hello, World!" 的程序：

```llvm
@.str = constant [14 x i8] c"Hello, World!\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %msg = getelementptr [14 x i8], [14 x i8]* @.str, i64 0, i64 0
  call i32 @printf(i8* %msg)
  ret i32 0
}
```

### 2. 循环结构
实现一个计算 1 到 10 累加和的程序：

```llvm
define i32 @main() {
entry:
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %sum
  store i32 1, i32* %i
  br label %loop.cond

loop.cond:
  %i_val = load i32, i32* %i
  %cmp = icmp sle i32 %i_val, 10
  br i1 %cmp, label %loop.body, label %loop.end

loop.body:
  %sum_val = load i32, i32* %sum
  %i_val2 = load i32, i32* %i
  %new_sum = add i32 %sum_val, %i_val2
  store i32 %new_sum, i32* %sum
  %new_i = add i32 %i_val2, 1
  store i32 %new_i, i32* %i
  br label %loop.cond

loop.end:
  %final_sum = load i32, i32* %sum
  ret i32 %final_sum
}
```

### 3. 函数调用
实现一个带函数调用的程序：

```llvm
define i32 @add(i32 %a, i32 %b) {
entry:
  %result = add i32 %a, %b
  ret i32 %result
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  store i32 10, i32* %x
  store i32 20, i32* %y

  %x_val = load i32, i32* %x
  %y_val = load i32, i32* %y

  %sum = call i32 @add(i32 %x_val, i32 %y_val)
  ret i32 %sum
}
```

### 4. 结构体的声明与使用
实现一个包含结构体的程序，定义一个 `Point` 结构体并计算两个点的欧几里得距离：

```llvm
%struct.Point = type { float, float }

define float @distance(%struct.Point* %p1, %struct.Point* %p2) {
entry:
  %x1_ptr = getelementptr %struct.Point, %struct.Point* %p1, i32 0, i32 0
  %y1_ptr = getelementptr %struct.Point, %struct.Point* %p1, i32 0, i32 1
  %x2_ptr = getelementptr %struct.Point, %struct.Point* %p2, i32 0, i32 0
  %y2_ptr = getelementptr %struct.Point, %struct.Point* %p2, i32 0, i32 1

  %x1 = load float, float* %x1_ptr
  %y1 = load float, float* %y1_ptr
  %x2 = load float, float* %x2_ptr
  %y2 = load float, float* %y2_ptr

  %dx = fsub float %x1, %x2
  %dy = fsub float %y1, %y2
  %dx2 = fmul float %dx, %dx
  %dy2 = fmul float %dy, %dy
  %sum = fadd float %dx2, %dy2
  %result = call float @llvm.sqrt.f32(float %sum)
  ret float %result
}

declare float @llvm.sqrt.f32(float)

define i32 @main() {
entry:
  %p1 = alloca %struct.Point, align 4
  %p2 = alloca %struct.Point, align 4

  %p1_x = getelementptr %struct.Point, %struct.Point* %p1, i32 0, i32 0
  %p1_y = getelementptr %struct.Point, %struct.Point* %p1, i32 0, i32 1
  store float 1.0, float* %p1_x
  store float 2.0, float* %p1_y

  %p2_x = getelementptr %struct.Point, %struct.Point* %p2, i32 0, i32 0
  %p2_y = getelementptr %struct.Point, %struct.Point* %p2, i32 0, i32 1
  store float 4.0, float* %p2_x
  store float 6.0, float* %p2_y

  %dist = call float @distance(%struct.Point* %p1, %struct.Point* %p2)
  ret i32 0
}
```

---

## 工具链与使用
1. **安装 LLVM**
   在大多数系统上可以通过包管理器安装 LLVM。例如：
   ```bash
   sudo apt install llvm clang
   ```

2. **生成 LLVM IR**
   使用 clang 编译 C 代码为 LLVM IR：
   ```bash
   clang -S -emit-llvm file.c -o file.ll
   ```

3. **执行 LLVM IR**
   使用 `lli` 直接解释执行 IR 文件：
   ```bash
   lli file.ll
   ```

4. **将 LLVM IR 编译为机器代码**
   使用 `llc` 或 `clang`：
   ```bash
   llc file.ll -o file.s     # 生成汇编代码
   clang file.s -o file.out  # 链接生成可执行文件
   ```

---

## 参考资源
- [LLVM 官方文档](https://llvm.org/docs/)
- [LLVM Language Reference Manual](https://llvm.org/docs/LangRef.html)
- [LLVM IR入门指南 by Evian Zhang](https://evian-zhang.github.io/llvm-ir-tutorial/)






```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Transforms/PassManagerBuilder.h>

#include "program_manager.h"

/**
 * Lower a single field (variable) to LLVM IR.
 */
static void convert_field_to_llvm(LLVMModuleRef module, LLVMBuilderRef builder, field_def *field) {
    const char *field_name = field->name;
    LLVMTypeRef llvm_type = LLVMInt32Type(); // Example: Assuming all fields are int32 for simplicity.
    
    // Create a global variable for the field.
    LLVMValueRef global_var = LLVMAddGlobal(module, llvm_type, field_name);
    LLVMSetInitializer(global_var, LLVMConstInt(llvm_type, 0, 0)); // Default to 0.
    
    printf("  [LLVM] Added global variable '%s'.\n", field_name);
}

/**
 * Lower a single function to LLVM IR.
 */
static void convert_func_to_llvm(LLVMModuleRef module, LLVMBuilderRef builder, func_def *fn) {
    const char *func_name = fn->name;

    // Example: Define function signature.
    LLVMTypeRef return_type = LLVMInt32Type(); // Assuming int32 return type for simplicity.
    LLVMTypeRef param_types[] = {};            // Assuming no parameters for simplicity.
    LLVMTypeRef func_type = LLVMFunctionType(return_type, param_types, 0, 0);
    
    // Add the function to the module.
    LLVMValueRef llvm_func = LLVMAddFunction(module, func_name, func_type);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(llvm_func, "entry");
    LLVMPositionBuilderAtEnd(builder, entry);
    
    // Example: Return 0 for all functions.
    LLVMValueRef ret_val = LLVMConstInt(return_type, 0, 0);
    LLVMBuildRet(builder, ret_val);

    printf("  [LLVM] Added function '%s'.\n", func_name);
}

/**
 * Lower a single struct to LLVM IR.
 */
static void convert_struct_to_llvm(LLVMModuleRef module, struct_def *st) {
    const char *struct_name = st->name;

    // Example: Define struct type (empty for simplicity).
    LLVMTypeRef struct_type = LLVMStructCreateNamed(LLVMGetGlobalContext(), struct_name);
    LLVMTypeRef element_types[] = { LLVMInt32Type(), LLVMFloatType() }; // Example: int32 and float.
    LLVMStructSetBody(struct_type, element_types, 2, 0);

    printf("  [LLVM] Defined struct '%s'.\n", struct_name);
}

/**
 * Recursively convert everything in a given scope to LLVM IR.
 */
static void convert_scope_to_llvm(program_manager *pm, scope *sc, LLVMModuleRef module, LLVMBuilderRef builder) {
    if (!sc) return;

    // Convert fields in this scope.
    size_t field_count = scope_get_field_count(sc);
    for (size_t i = 0; i < field_count; i++) {
        field_def *f = scope_get_field_by_index(sc, i);
        convert_field_to_llvm(module, builder, f);
    }

    // Convert functions in this scope.
    size_t func_count = scope_get_func_count(sc);
    for (size_t i = 0; i < func_count; i++) {
        func_def *fn = scope_get_func_by_index(sc, i);
        convert_func_to_llvm(module, builder, fn);
    }

    // Convert structs in this scope.
    size_t struct_count = scope_get_struct_count(sc);
    for (size_t i = 0; i < struct_count; i++) {
        struct_def *st = scope_get_struct_by_index(sc, i);
        convert_struct_to_llvm(module, st);
    }

    // Recurse into subscopes.
    size_t subscope_count = program_manager_get_subscope_count(pm);
    for (size_t i = 0; i < subscope_count; i++) {
        scope *sub = program_manager_get_subscope(pm, i);
        convert_scope_to_llvm(pm, sub, module, builder);
    }
}

/**
 * High-level driver to convert an entire program to LLVM IR.
 */
LLVMModuleRef program_manager_convert_to_llvm_ir(program_manager *pm) {
    // Create an LLVM module.
    LLVMModuleRef module = LLVMModuleCreateWithName("program");
    LLVMBuilderRef builder = LLVMCreateBuilder();

    // Convert the global scope recursively.
    convert_scope_to_llvm(pm, pm->global_scope, module, builder);

    // Cleanup the builder.
    LLVMDisposeBuilder(builder);

    return module;
}

/**
 * Example usage:
 */
int main() {
    // Create a program manager.
    program_manager *pm = program_manager_new();

    // (Hypothetical) Add some fields, funcs, structs...
    type_def *int_type = ...; // Assume type_def for int32.
    program_manager_create_field(pm, "g_x", int_type);
    program_manager_create_func(pm, "foo", int_type);
    program_manager_create_struct(pm, "Bar");

    // Convert to LLVM IR.
    LLVMModuleRef module = program_manager_convert_to_llvm_ir(pm);

    // Print the LLVM IR.
    char *ir_str = LLVMPrintModuleToString(module);
    printf("%s\n", ir_str);
    LLVMDisposeMessage(ir_str);

    // Write to file.
    LLVMWriteBitcodeToFile(module, "output.bc");

    // Cleanup.
    LLVMDisposeModule(module);
    program_manager_free(pm);

    return 0;
}
```

