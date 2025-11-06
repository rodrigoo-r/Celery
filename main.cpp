#include "celery/io/output.h"
#include "celery/ptr/shared.h"
#include "celery/ptr/unique.h"

struct MyClass
    : Celery::Base::Display
{
    [[nodiscard]] Celery::Str::String ToString()
        const noexcept override
    {
        return "MyClass(2)";
    }
};

void copy(Celery::Ptr::Shared<MyClass> c)
{

}

int main()
{
    auto a = Celery::Ptr::MakeShared<MyClass>();
    auto b = Celery::Ptr::MakeUnique<MyClass>();
    auto d = Celery::Ptr::MakeConcurrent<MyClass>();
    Celery::Ptr::Shared<MyClass> c = a;
    copy(a);

    Celery::Io::Println("Obj: ", *a);
    Celery::Io::Println("Obj: ", *b);
    return 0;
}