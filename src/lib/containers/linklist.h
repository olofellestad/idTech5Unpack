//--------------------------------------------------------------------------------------------------------------------------------
//  linklist.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

template< class Type >
class TLink
{
public:
    constexpr TLink( TLink *_prev = nullptr,
                     TLink *_next = nullptr )
        : prev( _prev )
        , next( _next )
    {}
    
public:
    TLink< Type > *prev;
    TLink< Type > *next;
};

template< class Type >
class TLinkList
{
    MAKE_NONCOPYABLE( TLinkList );
    
public:
    constexpr TLinkList();
    TLinkList( TLinkList &&other ) noexcept;
    
    ~TLinkList();
    
    constexpr bool IsEmpty() const;
    
    constexpr const TLink< Type > *GetHead() const;
    TLink< Type > *                GetHead();
    constexpr const TLink< Type > *GetTail() const;
    TLink< Type > *                GetTail();
    
    void Append( Type *value );
    
    void Remove( Type *value );
    void Clear();
    
    TLinkList &operator=( TLinkList &&other ) noexcept;
    
private:
    TLink< Type > head;
};

//--------------------------------------------------------------------------------------------------------------------------------
template< class Type >
constexpr TLinkList< Type >::TLinkList()
    : head( &head, &head )
{
}

template< class Type >
TLinkList< Type >::TLinkList( TLinkList &&other ) noexcept
    : head( other.head )
{
    other.head.next = &other.head;
    other.head.prev = &other.head;
}

template< class Type >
TLinkList< Type >::~TLinkList()
{
    Clear();
}

template< class Type >
constexpr bool TLinkList< Type >::IsEmpty() const
{
    return head.next == &head;
}

template< class Type >
constexpr const TLink< Type > *TLinkList< Type >::GetHead() const
{
    return head.next;
}

template< class Type >
TLink< Type > *TLinkList< Type >::GetHead()
{
    return head.next;
}

template< class Type >
constexpr const TLink< Type > *TLinkList< Type >::GetTail() const
{
    return head.prev;
}

template< class Type >
TLink< Type > *TLinkList< Type >::GetTail()
{
    return head.prev;
}

template< class Type >
void TLinkList< Type >::Append( Type *value )
{
    ASSERT( value != nullptr );
    
    TLink< Type > *link = value;
    
    link->prev      = head.prev;
    link->next      = &head;
    head.prev->next = link;
    head.prev       = link;
}

template< class Type >
void TLinkList< Type >::Remove( Type *value )
{
    if ( value == nullptr ) {
        return;
    }
    
    TLink< Type > *link = value;
    
    if ( link->next ) link->next->prev = link->prev;
    if ( link->prev ) link->prev->next = link->next;

    link->prev = link->next = nullptr;
}

template< class Type >
void TLinkList< Type >::Clear()
{
    while ( !IsEmpty() ) {
        Remove( (Type *)head.next );
    }
}

template< class Type >
TLinkList< Type > &TLinkList< Type >::operator=( TLinkList &&other ) noexcept
{
    if ( this == &other ) {
        return *this;
    }
    
    head			= other.head;
    other.head.next = &other.head;
    other.head.prev = &other.head;
    
    return *this;
}
