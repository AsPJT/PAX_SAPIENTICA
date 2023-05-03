# Contribution Guide

## PAX_SAPIENTICA コーディング規約

PAX_SAPIENTICAへ新しい機能を追加する方は以下のコーディング規約のもとにソースコードを追加してください。

---

# 命名法

## クラス名/構造体名/Enumの名前

パスカルケース(アッパーキャメルケース)にしてください。

### 正しい例

```cpp
class Sample {};
class SampleClass {};
class SampleClass2 {};
class SampleTestTest {};
```

### 間違い例

```cpp
class sample {};
class sampleClass {};
class sample_class {};
class Sample_Class {};
```

## 関数名

関数名はローワーキャメルケースにしてください。

### 正しい例

```cpp
void sample();
void sampleClass();
void sampleClass2();
void sampleTestTest();
```

### 間違い例

```cpp
void Sample();
void SampleClass();
void sample_class();
void sample_Class();
void Sample_Class();
```

## 変数名/Enumの中身

すべて小文字のスネークケースにしてください。

## マクロ名

すべて大文字のスネークケースにしてください。


## C++ファイル名

ファイル名はパスカルケース(アッパーキャメルケース)にしてください。
また拡張子として末尾に **`.hpp`** をつけてください。

### 正しい例

```cpp
Sample.hpp
SampleClass.hpp
SampleClass2.hpp
SampleTestTest.hpp
```

### 間違い例

```cpp
sample.hpp
sampleClass.hpp
sample_class.hpp
Sample_Class.hpp
SampleClass.cpp
SampleTestTest.h
```

---

# 型

## 整数型

C/C++で使われる一般の整数型を使用しています。

## 配列の添え字の型

配列の添え字に使用する型は **`std::size_t`** です。

# マクロ

## マクロの形式

PAX_SAPIENTICAでは以下の2種類のマクロを使用しています。

|マクロの種類|説明|
|:---|:---|
|PAX_SAPIENTICA_XXX_XXX_HPP|インクルードガードに使用。|
|PAX_SAPIENTICA_LIBRARY_XXX|バージョン情報などのマクロに使用。|

## PAX_SAPIENTICA_XXX_XXX_HPPマクロ

インクルードとして使用しています。

```cpp
#ifndef PAX_SAPIENTICA_CONSTANT_VERSION_HPP
#define PAX_SAPIENTICA_CONSTANT_VERSION_HPP

// 中身

#endif // !PAX_SAPIENTICA_CONSTANT_VERSION_HPP
```

# 禁止事項

## using namespace

hppファイル内では使用しないでください。

# 非推奨

 クラス内/構造体内/関数内以外でのusing/typedefの使用

