#ifndef _template_counter_h_
#define _template_counter_h_
#else
#error reinclude template_counter.h
#endif

// http://stackoverflow.com/questions/6166337/does-c-support-compile-time-counters

#define COUNTER_( TAG, RANK, ACC ) counter_crumb( TAG(), constant_index< RANK >(), constant_index< ACC >() )
#define COUNTER( TAG ) COUNTER_( TAG, 1, COUNTER_( TAG, 2, COUNTER_( TAG, 4, COUNTER_( TAG, 8, COUNTER_( TAG, 16, COUNTER_( TAG, 32, COUNTER_( TAG, 64, COUNTER_( TAG, 128, 0 ) ) ) ) ) ) ) )
#define COUNTER_INC( TAG ) constexpr constant_index< COUNTER( TAG ) + 1 > counter_crumb( TAG, constant_index< ( COUNTER( TAG ) + 1 ) & ~ COUNTER( TAG ) >, constant_index< ( COUNTER( TAG ) + 1 ) & COUNTER( TAG ) > ) { return {}; }
#define COUNTER_LINK_NAMESPACE( NS ) using NS::counter_crumb;

template< std::size_t n > struct constant_index : std::integral_constant< std::size_t, n > {};
template< typename id, std::size_t rank, std::size_t acc > constexpr constant_index< acc > counter_crumb(id, constant_index< rank >, constant_index< acc >) { return{}; } // found by ADL via constant_index
