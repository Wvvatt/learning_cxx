[toc]
## 文件命名

小写蛇形
- `this_is_header.h`
- `this_is_c_file.c` 

## 类型命名

大驼峰，包括类、结构体、类型定义（typedef）、别名、枚举、类型模板参数

```cpp
// 类和结构体
class UrlTable { ...
class UrlTableTester { ...
struct UrlTableProperties { ...

// 类型定义
typedef hash_map<UrlTableProperties *, string> PropertiesMap;

// using 别名
using PropertiesMap = hash_map<UrlTableProperties *, string>;

// 枚举
enum UrlTableErrors { ...
```

## 变量命名

小写蛇形：变量、数据成员（类的成员使用下划线结尾，结构体不用）

```cpp
string table_name;  // 普通变量

class TableInfo {   // 类，后加下划线
  ...
 private:
  string table_name_;  
  static Pool<TableInfo>* pool_;  
};

struct UrlTableProperties {     // 结构体，不加下划线
  std::string name;
  int num_entries;
  static Pool<UrlTableProperties>* pool;
};
```

## 常量命名

以'k'开头的大驼峰：`const` `constexpr`
```cpp
const int kDaysInAWeek = 7;
constexpr int kDaysInAWeek = 7;
```

## 函数命名

- 类方法：大驼峰
- 普通函数：小蛇行或者大驼峰
- setter、getter：小蛇行与变量名匹配

```cpp
class MyObject
{
public:
    void Method();
    void set_value_of_life(int);
    int value_of_life();
private:
    int value_of_life;
}

int callback_write();

```

## 宏命名

大写蛇形
```cpp
#define MY_MACRO_THAT_SCARES_SMALL_CHILDREN 1
```

## 枚举名称

与常量或者宏一致
```cpp
enum UrlTableErrors {
    kOK = 0,
    kErrorOutOfMemory,
    kErrorMalformedInput,
};
enum AlternateUrlTableErrors {
    OK = 0,
    OUT_OF_MEMORY = 1,
    MALFORMED_INPUT = 2,
};
```

## 命名空间命名

小写蛇形
