#ifndef REFLECTION_H
#define REFLECTION_H

#include <cstddef>
#include <typeinfo>
#include <stdint.h>
#include <string.h>

namespace TypeTraits /* C++11's type_traits emulation */
{
    template<bool COND, typename T = void> struct if_ { };
    template<typename T> struct if_<true, T> { typedef T type; };

    template<typename T, T v> struct typed_constant { typedef T value_type; static const T value = v; };
    typedef typed_constant<bool, false> false_type;
    typedef typed_constant<bool, true> true_type;

    template<typename T> struct is_pointer: public false_type { };
    template<typename T> struct is_pointer<T*>: public true_type { };

    template<typename T> struct is_boolean: public false_type { };
    template<> struct is_boolean<bool>: public true_type { };

    template<typename T> struct is_integral: public false_type { };
    template<> struct is_integral<bool>: public true_type { };
    template<> struct is_integral<char>: public true_type { };
    template<> struct is_integral<wchar_t>: public true_type { };
    template<> struct is_integral<long int>: public true_type { };
    template<> struct is_integral<long long int>: public true_type { };
    template<> struct is_integral<unsigned long int>: public true_type { };
    template<> struct is_integral<unsigned long long int>: public true_type { };
    template<> struct is_integral<int8_t>: public true_type { };
    template<> struct is_integral<int16_t>: public true_type { };
    template<> struct is_integral<int32_t>: public true_type { };
    template<> struct is_integral<uint8_t>: public true_type { };
    template<> struct is_integral<uint16_t>: public true_type { };
    template<> struct is_integral<uint32_t>: public true_type { };

    template<typename T> struct is_signed: public false_type { };
    template<> struct is_signed<char>: public true_type { };
    template<> struct is_signed<wchar_t>: public true_type { };
    template<> struct is_signed<long int>: public true_type { };
    template<> struct is_signed<long long int>: public true_type { };
    template<> struct is_signed<int8_t>: public true_type { };
    template<> struct is_signed<int16_t>: public true_type { };
    template<> struct is_signed<int32_t>: public true_type { };
    template<> struct is_signed<float>: public true_type { };
    template<> struct is_signed<double>: public true_type { };
    template<> struct is_signed<long double>: public true_type { };

    template<typename T> struct is_floating_point: public false_type { };
    template<> struct is_floating_point<float>: public true_type { };
    template<> struct is_floating_point<double>: public true_type { };
    template<> struct is_floating_point<long double>: public true_type { };

    template<typename T> struct is_array: public TypeTraits::false_type { static const bool valid = false; static const size_t size = 1; };
    template<typename T> struct is_array<T[]>: public TypeTraits::true_type { static const bool valid = false; static const size_t size = 1; };
    template<typename T, size_t S> struct is_array<T[S]>: public TypeTraits::true_type { static const bool valid = true; static const size_t size = S; };

    template<typename T> struct is_string: public false_type { };
    template<> struct is_string<char*>: public true_type { };
    template<> struct is_string<char[]>: public true_type { };
    template<size_t S> struct is_string<char[S]>: public true_type { };
    template<> struct is_string<const char*>: public true_type { };
    template<> struct is_string<const char[]>: public true_type { };
    template<size_t S> struct is_string<const char[S]>: public true_type { };

    template<typename T> struct is_void: public false_type { };
    template<> struct is_void<void>: public true_type { };

    template<typename T> struct is_enum: public typed_constant<bool, __is_enum(T)> { };
    template<typename T> struct is_union: public typed_constant<bool, __is_union(T)> { };
    template<typename T> struct is_class: public typed_constant<bool, __is_class(T)> { };

    template<typename T> struct is_struct: false_type { };

    template<int S> struct fit_to_signed_bytes { };
    template<> struct fit_to_signed_bytes<1> { typedef int8_t Type; };
    template<> struct fit_to_signed_bytes<2> { typedef int16_t Type; };
    template<> struct fit_to_signed_bytes<4> { typedef int32_t Type; };
    template<> struct fit_to_signed_bytes<8> { typedef int64_t Type; };

    template<int S> struct fit_to_unsigned_bytes { };
    template<> struct fit_to_unsigned_bytes<1> { typedef uint8_t Type; };
    template<> struct fit_to_unsigned_bytes<2> { typedef uint16_t Type; };
    template<> struct fit_to_unsigned_bytes<4> { typedef uint32_t Type; };
    template<> struct fit_to_unsigned_bytes<8> { typedef uint64_t Type; };
}

namespace TypeTemplates
{
    template<typename T, typename S, typename FI, typename E = void> struct FieldValueT
    {
        static T value(S* s, const FI* fi)
        {
            return *((T*)((size_t)s + fi->Offset));
        }
    };

    template<typename T, typename S, typename FI> struct FieldValueT<T, S, FI, typename TypeTraits::if_< TypeTraits::is_pointer<T>::value >::type >
    {
        static T value(S* s, const FI* fi)
        {
            return (T)((size_t)s + fi->Offset);
        }
    };

    template<typename T, typename S, typename FI> struct FieldValueT<T, S, FI, typename TypeTraits::if_< TypeTraits::is_integral<T>::value >::type >
    {
        static T value(S* s, const FI* fi)
        {
            if(TypeTraits::is_signed<T>::value)
            {
                switch(fi->TypeSize)
                {
                    case 1:
                        return *((TypeTraits::fit_to_signed_bytes<1>::Type*)(((size_t)s) + fi->Offset));

                    case 2:
                        return *((TypeTraits::fit_to_signed_bytes<2>::Type*)(((size_t)s) + fi->Offset));

                    case 4:
                        return *((TypeTraits::fit_to_signed_bytes<4>::Type*)(((size_t)s) + fi->Offset));

                    case 8:
                        return *((TypeTraits::fit_to_signed_bytes<8>::Type*)(((size_t)s) + fi->Offset));

                    default:
                        break;
                }
            }
            else
            {
                switch(fi->TypeSize)
                {
                    case 1:
                        return *((TypeTraits::fit_to_unsigned_bytes<1>::Type*)(((size_t)s) + fi->Offset));

                    case 2:
                        return *((TypeTraits::fit_to_unsigned_bytes<2>::Type*)(((size_t)s) + fi->Offset));

                    case 4:
                        return *((TypeTraits::fit_to_unsigned_bytes<4>::Type*)(((size_t)s) + fi->Offset));

                    case 8:
                        return *((TypeTraits::fit_to_unsigned_bytes<8>::Type*)(((size_t)s) + fi->Offset));

                    default:
                        break;
                }
            }

            return *((T*)(((size_t)s) + fi->Offset));
        }
    };
}

namespace TypeFunctions
{
    template<typename SI, typename FI> bool containsFieldT(const char* fieldname)
    {
        const FI* fi = SI::Fields;

        for(size_t i = 0; i < SI::Count; i++)
        {
            if(!strcmp(fi[i].Name, fieldname))
                return true;
        }

        return false;
    }

    template<typename SI, typename FI> const FI* getFieldT(const char* fieldname)
    {
        const FI* fi = SI::Fields;

        for(size_t i = 0; i < SI::Count; i++)
        {
            if(!strcmp(fi[i].Name, fieldname))
                return &fi[i];
        }

        throw NULL;
    }
}

template<typename T, typename S, typename FI> T fieldValue(S* s, const FI* fi)
{
    return TypeTemplates::FieldValueT<T, S, FI>::value(s, fi);
}

#define ArraySize(array) sizeof(array) / sizeof(array[0])
#define StructInfoType(name) name ## _StructureInfo
#define FieldInfoType(name) name ## _FieldInfo

#define DeclareIsStructTrait(name) namespace TypeTraits { \
                                    template<> struct is_struct<name>: public true_type { }; \
                                    template<> struct is_struct<name*>: public true_type { }; \
                                    template<> struct is_struct<const name*>: public true_type { }; \
                                  }

#define DeclareFieldInfo(name) struct FieldInfoType(name) { \
                                const std::type_info& TypeInfo; \
                                const char* const TypeName; \
                                const char* const Name; \
                                const size_t TypeSize; \
                                const size_t Offset; \
                                \
                                const size_t ArraySize; \
                                const bool ArraySizeValid; \
                                \
                                const bool IsArray; \
                                const bool IsBoolean; \
                                const bool IsString; \
                                const bool IsIntegral; \
                                const bool IsFloatingPoint; \
                                const bool IsPointer; \
                                const bool IsClass; \
                                const bool IsStruct; \
                                const bool IsUnion; \
                                const bool IsEnum; \
                                };

#define DeclareStructureInfo(name) DeclareIsStructTrait(name) \
                                \
                                DeclareFieldInfo(name) \
                                \
                                struct StructInfoType(name) { \
                                    static const char* const Name; \
                                    static const FieldInfoType(name) Fields[]; \
                                    static const size_t Count; \
                                };

#define BeginStructureInfo(name) const char* const StructInfoType(name)::Name = #name; \
                             const FieldInfoType(name) StructInfoType(name)::Fields[] = {

#define EndStructureInfo(name) }; \
                            const size_t StructInfoType(name)::Count = ArraySize(Fields);


#define TypeInfo(type) TypeTraits::is_array<type>::value,\
                       TypeTraits::is_boolean<type>::value,\
                       TypeTraits::is_string<type>::value,\
                       TypeTraits::is_integral<type>::value,\
                       TypeTraits::is_floating_point<type>::value, \
                       TypeTraits::is_pointer<type>::value, \
                       TypeTraits::is_class<type>::value, \
                       TypeTraits::is_struct<type>::value, \
                       TypeTraits::is_union<type>::value, \
                       TypeTraits::is_enum<type>::value

#define StructureField(name, type, fieldname) { typeid(type), \
                                       #type, \
                                       #fieldname, \
                                       sizeof(type), \
                                       offsetof(name, fieldname), \
                                       TypeTraits::is_array<type>::size, \
                                       TypeTraits::is_array<type>::valid, \
                                       TypeInfo(type) }

//#define FieldValue(name, s, type, field) fieldValue<type>(s, offsetof(name, field))
#define ContainsField(name, field) TypeFunctions::containsFieldT<StructInfoType(name), FieldInfoType(name)>(field)
#define GetField(name, field) TypeFunctions::getFieldT<StructInfoType(name), FieldInfoType(name)>(field)
#define IsField(field1, field2) !strcmp(field1->Name, field2)

#endif // REFLECTION_H

