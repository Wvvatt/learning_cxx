#include <iostream>
//https://stackoverflow.com/questions/13697265/static-const-double-cannot-have-an-in-class-initializer-why-is-it-so

/* 
- 声明、定义、初始化是三个过程，一般初始化是定义的一部分，也就是发生初始化时必须已定义，定义时分配内存。
- 类中数据如果不初始化，只属于声明，那么可以出现在多个编译单元中。
- 静态常量/变量在类内初始化相当于同时完成了定义、初始化，有了实例，那么出现在多编译单元中时，会存在多个实例。因此不可以类内初始化。
- 唯一的例外是静态常量整形，因为它满足Integral Constant Expressions (ICEs)的要求，在编译时会替换成右值，因此必须在每个编译单元中可见，因此可以出现在声明中。

- 类生成对象一般对应着三个过程：声明表示类的内存分布 --> 申请对象时开辟内存 --> 构造函数之前进行初始化。
- 非静态成员变量/常量归属于每一个对象的内存，有多个实例，因此可以分别初始化。
- c++11支持使用类内初始化对类内非静态数据成员进行初始化，这种方式看似是初始化，实际只是相当于声明一种初始化方式，实际初始化还是发生在运行时定义对象后。
- 初始化列表与类内初始化不冲突，可以同时使用。初始化列表发生在类内初始化之后。

- 最好的写法就是只把struct和class中的内容当作声明。
*/

class InPlaceInit
{
private:
// 静态成员类内初始化，会生成实例
    // 静态整型常量作为ICEs，可以类内初始化
    static const int sc_data = 100;                     
    // 其他静态常量类内初始化需要在前面添加constexpr，只有const不行
    static constexpr const char *sc_name = "hello";     
    static constexpr float sc_float = 200.0;      
    // 静态变量只能声明，不可以在类内初始化
    static char *s_name;                                
    static int s_data;                                  

// 非静态成员类内初始化，相当于声明一种初始化方式，因此可以类内初始化
    const char *c_name = "hello";                    
    const int c_data = 100;                           
    const char *name = "hello";                              
    int data = 100;                                     
};

class AVClass
{
public:
    virtual const char *class_name() const = 0;
    virtual int data() const = 0;
};

// 类内常量在构造前采用类内初始化
class MpegTsClass : public AVClass
{
public:
    const char *class_name() const override { return class_name_; };
    int data() const override { return data_; }

private:
    const char *class_name_ = "mpeg_ts_class";
    const int data_ = 200;
};

// 类内常量在构造前采用初始化列表初始化
class HlsClass : public AVClass
{
public:
    HlsClass(const char *s, int a) : class_name_(s), data_(a) {};
    const char *class_name() const override { return class_name_; };
    int data() const override { return data_; }

private:
    const char *class_name_;
    const int data_;
};

// 类内常量在构造函数内 赋值 -->  无法编译 
// class FlvClass : public AVClass
// {
// public:
//     FlvClass(const char *s, int data){
//         class_name_ = s;
//         data_ = data;
//     };
//     const char *class_name() const override { return class_name_; };
//     int data() const override { return data_; }

// private:
//     const char *class_name_;
//     const int data_ ;
// };

int main()
{
    AVClass *hls = new HlsClass("hls_class", 100);
    AVClass *mpegts = new MpegTsClass;
    std::cout << hls->class_name() << " " << hls->data() << std::endl;
    std::cout << mpegts->class_name() << " " << mpegts->data() << std::endl;
};
