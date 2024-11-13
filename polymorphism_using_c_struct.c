#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int(*eat)(MyObject *);
    int(*run)(MyObject *);
}MyObjectClass;

typedef struct
{
    const char *name;
}MyObject;

static int
my_object_eat(MyObject *obj)
{
    printf("parent eat %s\n", obj->name);
    return 1;
};

static int
my_object_run(MyObject *obj)
{
    printf("parent run %s\n", obj->name);
    return 1;
};

void my_object_class_init(MyObjectClass *class)
{
    class->eat = my_object_eat;
    class->run = my_object_run;
};

void my_object_init(MyObject *obj)
{
    obj->name = "MyObject";
};

typedef struct
{
    MyObjectClass *parent;
}MyObjectDerivedClass;

typedef struct
{
    MyObject parent;
    int age;
}MyObjectDerived;

int my_object_derived_eat(MyObject *obj)
{
    printf("child eat %s\n", obj->name);
    return 1;
};

void my_object_derived_class_init(MyObjectClass *class)
{
    class->eat = my_object_derived_eat;
}

void my_object_derived_init(MyObject *obj)
{
    MyObjectDerived *derived_obj = (MyObjectDerived *)obj;
    obj->name = "MyObjectDerived";
    derived_obj->age = 10;
}

int main()
{
    MyObjectClass *class = (MyObjectClass *)malloc(sizeof(MyObjectClass));
    MyObject *obj = (MyObject *)malloc(sizeof(MyObject));
    my_object_class_init(class);
    my_object_init(obj);
    class->eat(obj);
    class->run(obj);

    free(obj);
    free(class);

    class = (MyObjectClass *)malloc(sizeof(MyObjectDerivedClass));
    obj = (MyObject *)malloc(sizeof(MyObjectDerived));
    my_object_class_init(class);
    my_object_init(obj);
    my_object_derived_class_init(class);
    my_object_derived_init(obj);
    class->eat(obj);
    class->run(obj);

    free(obj);
    free(class);

    return 0;
}