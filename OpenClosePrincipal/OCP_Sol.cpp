#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class Color
{
    red,
    green,
    blue
};
enum class Size
{
    small,
    medium,
    large
};

struct Product
{
    string name;
    Color color;
    Size size;
};

template <typename T>
struct AndSpecification;

template <typename T>
struct Specification
{
    virtual ~Specification() = default;
    virtual bool is_satisfied(T *item) const = 0;

    // new: breaks OCP if you add it post-hoc
    //AndSpecification<T> operator&&(Specification<T> &&other)
    //{
    //    return AndSpecification<T>(*this, other);
   // }
};

template <typename T>
struct Filter
{
    virtual vector<T *> filter(vector<T *> items, Specification<T> &spec) = 0;
};

struct BetterFilter : Filter<Product>
{
    vector<Product *> filter(vector<Product *> items,
                             Specification<Product> &spec) override
    {
        vector<Product *> results;
        for (auto &item : items)
            if (spec.is_satisfied(item))
                results.push_back(item);
        return results;
    }
};

struct ColorSpecification : Specification<Product>
{
    Color color;
    ColorSpecification(Color color) : color(color)
    {
    }

    bool is_satisfied(Product *item) const override
    {
        return item->color == color;
    }
};

struct SizeSpecification : Specification<Product>
{
    Size size;

    explicit SizeSpecification(const Size size)
        : size{size}
    {
    }

    bool is_satisfied(Product *item) const override
    {
        return item->size == size;
    }
};

template <typename T>
struct AndSpecification : Specification<T>
{
    const Specification<T> &first;
    const Specification<T> &second;

    AndSpecification(const Specification<T> &first, const Specification<T> &second)
        : first(first), second(second) {}

    bool is_satisfied(T *item) const override
    {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};

int main()
{
    Product apple{"Apple", Color::green, Size::small};
    Product tree{"Tree", Color::green, Size::large};
    Product house{"House", Color::blue, Size::large};
    const vector<Product *> items{&apple, &tree, &house};
    BetterFilter bf;

    ColorSpecification green(Color::green);
    SizeSpecification size(Size::large);
    for (auto &x : bf.filter(items, green))
    {
        cout << x->name << " is green\n";
    }
    for (auto &x : bf.filter(items, size))
    {
        cout << x->name << " is large\n";
    }

    AndSpecification<Product> spec(green,size);
    
    for (auto &x : bf.filter(items, spec))
    {
        cout << x->name << " is green and large\n";
    }

    getchar();
    return 0;
}
