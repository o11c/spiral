#ifndef COERCION_HPP
#define COERCION_HPP

namespace coercion
{
    template<class CRTP, class T>
    class operators
    {
        T _dummy[0];

        CRTP& _get() { return static_cast<CRTP&>(*this); }
        const CRTP& _get() const { return static_cast<const CRTP&>(*this); }
        T get() { return _get(); }
        const T& get() const { return _get(); }
        void set(const T& v) { _get() = v; }
    public:
        // Most operators will implicitly coerce themselves to T

        // binary in-place operators
        template<class U>
        friend operators& operator += (operators& o, const U& r)
        {
            o.set(o.get() + r);
            return o;
        }
        template<class U>
        friend operators& operator -= (operators& o, const U& r)
        {
            o.set(o.get() - r);
            return o;
        }
        template<class U>
        friend operators& operator *= (operators& o, const U& r)
        {
            o.set(o.get() * r);
            return o;
        }
        template<class U>
        friend operators& operator /= (operators& o, const U& r)
        {
            o.set(o.get() / r);
            return o;
        }
    };
}
#endif //COERCION_HPP
