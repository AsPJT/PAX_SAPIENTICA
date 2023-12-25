# Contribution Guide

## PAX_SAPIENTICA コーディング規約 coding rule

PAX_SAPIENTICAへ新しい機能を追加する方は以下のコーディング規約のもとにソースコードを追加してください。

Those adding new features should add source code under the following coding rules.

---

# 命名規則 Naming Rule

## クラス名/構造体名/Enumの名前 Class-Struct name/Enum name

パスカルケース(アッパーキャメルケース)にしてください。

Make it a Pascal case (upper camel case).

### 正しい例 Correct Example

```cpp
class Sample {};
class SampleClass {};
class SampleClass2 {};
class SampleTestTest {};
```

### 間違い例 Wrong Example

```cpp
class sample {};
class sampleClass {};
class sample_class {};
class Sample_Class {};
```

## 関数名 Function Name

関数名はローワーキャメルケースにしてください。

The function name should be Lower CamelCase.

### 正しい例 Correct Example

```cpp
void sample();
void sampleClass();
void sampleClass2();
void sampleTestTest();
```

### 間違い例 Wrong Example

```cpp
void Sample();
void SampleClass();
void sample_class();
void sample_Class();
void Sample_Class();
```

## 変数名/Enumの中身 Variable Name/Enum Contents

すべて小文字のスネークケースにしてください。

Make all lowercase snake case.

## マクロ名

すべて大文字のスネークケースにしてください。

Make all uppercase snake case.

## C++ファイル名 C++ file name

ファイル名はパスカルケース(アッパーキャメルケース)にしてください。
また拡張子として末尾に **`.hpp`** をつけてください。

The file name should be Pascal Case (Upper CamelCase).
Also, please add **`.hpp`** at the end as an extension.

### 正しい例 Correct Example

```cpp
Sample.hpp
SampleClass.hpp
SampleClass2.hpp
SampleTestTest.hpp
```

### 間違い例 Wrong Example

```cpp
sample.hpp
sampleClass.hpp
sample_class.hpp
Sample_Class.hpp
SampleClass.cpp
SampleTestTest.h
```

---

# 型 Data Type

## 整数型 Integer Type

C/C++で使われる一般の整数型を使用しています。

It uses the general integer type used in C/C++.

## 配列の添え字の型 Array subscript type

配列の添え字に使用する型は **`std::size_t`** です。

The type used for array subscripts is **`std::size_t`**.

# マクロ Macro

## マクロの形式 Macro format

PAX_SAPIENTICAでは以下の4種類のマクロを使用しています。

|マクロの種類|説明|
|:---|:---|
|PAX_SAPIENTICA_XXX_XXX_HPP|インクルードガードに使用。|
|PAX_SAPIENTICA_LIBRARY_XXX|バージョン情報などのマクロに使用。|
|PAX_SAPIENTICA_XXX|その他のマクロに使用。|
|PAXS_XXX|ライブラリ使用ユーザが定義する可能性の高いマクロに使用。|

**English**

The following four macros are used.

|Macro type|Description|
|:---|:---|
|PAX_SAPIENTICA_XXX_XXX_HPP|Used for include guards.|
|PAX_SAPIENTICA_LIBRARY_XXX|Used for macros such as version information.|
|PAX_SAPIENTICA_XXX|Used for other macros.|
|PAXS_XXX|Used for macros that are likely to be defined by the user using the library.|

## PAX_SAPIENTICA_XXX_XXX_HPP マクロ Macro

インクルードガードとして使用しています。

It is used as an include guard.

```cpp
#ifndef PAX_SAPIENTICA_CONSTANT_VERSION_HPP
#define PAX_SAPIENTICA_CONSTANT_VERSION_HPP

// 中身 Contents

#endif // !PAX_SAPIENTICA_CONSTANT_VERSION_HPP
```

## 標準Cライブラリの使用に関して Regarding the use of the standard C library
1. 標準C++ライブラリで新しく実装されている機能はCの標準ライブラリ**ではなく**、**C++の標準ライブラリを用いること**。
1. インクルードは「〇〇.h」**ではなく**、「c〇〇」を用いること。例えばmath.hではなくcmath。
1. マクロを除き名前空間stdを必ずつけること。例：std::abs(-1)

**English**

1. Newly implemented functions in the standard C++ library should be used instead of the standard C library.
1. Use "cXX" for includes, not "XX.h". For example, cmath instead of math.h.
1. Namespace std must be added except for macros. Example: std::abs(-1)

## インクルードの順番 Order of inclusion
1. C言語の標準ライブラリ C standard library
1. C++の標準ライブラリ C++ standard library
1. 外部ライブラリ External Libraries
1. PAX_SAPIENTICAライブラリ PAX_SAPIENTICA Library

同じグループのインクルードはアルファベット順に並べる。

Includes of the same group should be arranged in alphabetical order.

数字はアルファベットよりも優先順位が高い。

Numbers have a higher priority than alphabetic characters.

例： `0>9>A>Z`

# 禁止事項 Prohibitions

## using namespace

hppファイル内では使用しないでください。

Do not use within hpp files.

# 非推奨

 クラス内/構造体内/関数内以外でのusing/typedefの使用

 Use of using/typedef outside of classes/structures/functions
