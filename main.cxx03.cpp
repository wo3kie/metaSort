/*
 * File:   main.cxx03.cpp
 * Author: Lukasz Czerwinski
 * Website: www.lukaszczerwinski.pl
 */

// based on Type List written by Andrei Alexandrescu

#include <iostream>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class StaticInterface { StaticInterface(); ~StaticInterface(); };

struct emptyValue : StaticInterface {
    enum { value = 65535 };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template < int _head, typename _Tail >
struct ValueList : StaticInterface {

    typedef int Head;
    typedef _Tail Tail;

    enum { value = _head };
};

#define INT_LIST_1( V1 ) ValueList< V1, emptyValue >
#define INT_LIST_2( V1, V2 ) ValueList< V1, INT_LIST_1( V2 ) >
#define INT_LIST_3( V1, V2, V3 ) ValueList< V1, INT_LIST_2( V2, V3 ) >
#define INT_LIST_4( V1, V2, V3, V4 ) ValueList< V1, INT_LIST_3( V2, V3, V4 ) >
#define INT_LIST_5( V1, V2, V3, V4, V5 ) ValueList< V1, INT_LIST_4( V2, V3, V4, V5 ) >
#define INT_LIST_6( V1, V2, V3, V4, V5, V6 ) ValueList< V1, INT_LIST_5( V2, V3, V4, V5, V6 ) >
#define INT_LIST_7( V1, V2, V3, V4, V5, V6, V7 ) ValueList< V1, INT_LIST_6( V2, V3, V4, V5, V6, V7 ) >
#define INT_LIST_8( V1, V2, V3, V4, V5, V6, V7, V8 ) ValueList< V1, INT_LIST_7( V2, V3, V4, V5, V6, V7, V8 ) >
#define INT_LIST_9( V1, V2, V3, V4, V5, V6, V7, V8, V9 ) ValueList< V1, INT_LIST_8( V2, V3, V4, V5, V6, V7, V8, V9 ) >
#define INT_LIST_10( V1, V2, V3, V4, V5, V6, V7, V8, V9, V10 ) ValueList< V1, INT_LIST_9( V2, V3, V4, V5, V6, V7, V8, V9, V10 ) >

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template < typename _VList, int _index >
struct ValueAt;

template < int _head, typename _Tail >
struct ValueAt < ValueList < _head, _Tail >, 0 > : StaticInterface {
    enum { value = _head };
};

template < int _head, typename _Tail, int _index >
struct ValueAt < ValueList < _head, _Tail >, _index > : StaticInterface {
    enum { value = ValueAt< _Tail, _index - 1 >::value };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template < typename _VList, int _value, int _replaceWith >
struct Replace;

template < int _value, int _replaceWith >
struct Replace < emptyValue, _value, _replaceWith > : StaticInterface {
    typedef emptyValue ResultType;
};

template < int _head, typename _Tail, int _replaceWith >
struct Replace < ValueList < _head, _Tail >, _head, _replaceWith > : StaticInterface {
    enum { value = ValueList< _replaceWith, _Tail >::value };
    typedef ValueList< _replaceWith, _Tail > ResultType;
};

template < int _head, typename _Tail, int _value, int _replaceWith >
struct Replace < ValueList < _head, _Tail >, _value, _replaceWith > : StaticInterface {
    enum { value = ValueList< _head, Replace< _Tail, _value, _replaceWith > >::value };
    typedef ValueList< _head, typename Replace< _Tail, _value, _replaceWith >::ResultType > ResultType;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// It is not a real predicate, since returns greater values instead of `true' or `false'
template < int _i1, int _i2 >
struct Greater : StaticInterface {
    enum { value = 
        ( _i1 == emptyValue::value ) ? 
            _i2 : 
            ( _i2 == emptyValue::value ) ? 
                _i1 : 
                ( _i1 < _i2 ) ? 
                    _i2 : 
                        _i1 
    };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// It is not a real predicate, since returns less values instead of `true' or `false'
template < int _i1, int _i2 >
struct Less : StaticInterface {
    enum { value = 
        ( _i1 == emptyValue::value ) ? 
            _i2 : 
            ( _i2 == emptyValue::value ) ? 
                _i1 : 
                ( _i1 < _i2 ) ? 
                    _i1 : 
                        _i2 
    };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template < typename _VList, template < int, int > class _GetBetter >
struct TheBest;

template < template < int, int > class _GetBetter >
struct TheBest < emptyValue, _GetBetter > : StaticInterface {
    enum { value = emptyValue::value };
};

template < int _head, typename _Tail, template < int, int > class _GetBetter >
struct TheBest < ValueList < _head, _Tail >, _GetBetter > : StaticInterface {
    enum { value = _GetBetter< _head, TheBest< _Tail, _GetBetter >::value >::value };
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

template < typename _VList, template < int, int > class _GetBetter >
struct Sort;

template < template < int, int > class _GetBetter >
struct Sort < emptyValue, _GetBetter > : StaticInterface {
    enum { value = emptyValue::value };
    typedef emptyValue ResultType;
};

template < int _head, typename _Tail, template < int, int > class _GetBetter >
struct Sort < ValueList < _head, _Tail >, _GetBetter > : StaticInterface {
    enum { value = _GetBetter< _head, TheBest< _Tail, _GetBetter >::value >::value };
    typedef typename Replace< _Tail, value, _head >::ResultType UnsortedTail;
    typedef ValueList< value, typename Sort< UnsortedTail, _GetBetter >::ResultType > ResultType;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int main(){
    typedef Sort< INT_LIST_9( 1, 9, 2, 8, 3, 7, 4, 6, 5 ), Greater >::ResultType DiscendingSortedArray;
    
    std::cout
        << ValueAt< DiscendingSortedArray, 0 >::value << ' '
        << ValueAt< DiscendingSortedArray, 1 >::value << ' '
        << ValueAt< DiscendingSortedArray, 2 >::value << ' '
        << ValueAt< DiscendingSortedArray, 3 >::value << ' '
        << ValueAt< DiscendingSortedArray, 4 >::value << ' '
        << ValueAt< DiscendingSortedArray, 5 >::value << ' '
        << ValueAt< DiscendingSortedArray, 6 >::value << ' '
        << ValueAt< DiscendingSortedArray, 7 >::value << ' '
        << ValueAt< DiscendingSortedArray, 8 >::value
    << std::endl;
    
    typedef Sort< INT_LIST_9( 1, 9, 2, 8, 3, 7, 4, 6, 5 ), Less >::ResultType AscendingSortedArray;
    
    std::cout
        << ValueAt< AscendingSortedArray, 0 >::value << ' '
        << ValueAt< AscendingSortedArray, 1 >::value << ' '
        << ValueAt< AscendingSortedArray, 2 >::value << ' '
        << ValueAt< AscendingSortedArray, 3 >::value << ' '
        << ValueAt< AscendingSortedArray, 4 >::value << ' '
        << ValueAt< AscendingSortedArray, 5 >::value << ' '
        << ValueAt< AscendingSortedArray, 6 >::value << ' '
        << ValueAt< AscendingSortedArray, 7 >::value << ' '
        << ValueAt< AscendingSortedArray, 8 >::value
    << std::endl;
}

