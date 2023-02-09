#include <iostream>

/*
- 初始化列表与就地初始化不冲突，可以同时使用。初始化列表发生在之后。
- c++11支持使用就地初始化对类内非静态数据成员进行初始化
- 非静态成员的变量，每个实例都有各自的变量
*/

// class InPlaceInit
// {
// private:
//     static const char *name = "hello"; // 不可以。需要在前面添加constexpr
//     static const int data = 100; // 可以
    
//     static char *name = "xx";  // 不可以在类内初始化
//     static int data = 100;    // 不可以在类内初始化

//     const char *name = "hello"; // 可以
//     const int data = 100;       // 可以 

//     char *name = "hello";      // 可以
//     int data = 100;            // 可以
// }   

class AVClass
{
public:
    virtual const char *class_name() const = 0;
    virtual int data() const = 0;
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
// 类内常量在构造前采用就地初始化
class MpegTsClass : public AVClass
{
public:
    const char *class_name() const override { return class_name_; };
    int data() const override { return data_; }

private:
    const char *class_name_ = "mpeg_ts_class";
    const int data_ = 200;
};

// 类内常量在构造函数内赋值 -->  无法编译 
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
    AVClass *hls = new HlsClass("hls_class", 100);
    AVClass *mpegts = new MpegTsClass;
    std::cout << hls->class_name() << " " << hls->data() << std::endl;
    std::cout << mpegts->class_name() << " " << mpegts->data() << std::endl;
};