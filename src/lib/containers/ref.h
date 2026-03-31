//--------------------------------------------------------------------------------------------------------------------------------
//  ref.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

// TODO: make atomic?
class RefCount
{
public:
	ALWAYS_INLINE void Acquire() const { ASSERT( ref > 0 ); ++ref; }
	ALWAYS_INLINE void Release() const { ASSERT( ref > 0 ); --ref; }

	uint GetRef() const { return ref; }

private:
	mutable uint ref = 1;
};

template< class Type >
class TRef
{
public:
	template< class... Args >
	static TRef< Type > Make( Args&&... args )
	{
		Type *ptr = new Type( std::forward< Args >( args )... );
    	return TRef< Type >( ptr );
	}

public:
	TRef();
	TRef( std::nullptr_t );
	TRef( Type *_ptr );
	TRef( TRef &&other ) noexcept;
	TRef( const TRef &other );

	~TRef();

    constexpr bool IsValid() const;

    const Type *GetPtr() const;
    Type *      GetPtr();

    const Type *operator->() const;
    Type *      operator->();
    const Type &operator*() const;
    Type &      operator*();
    
    constexpr bool operator==( const TRef &other ) const;
    constexpr bool operator!=( const TRef &other ) const;

    constexpr bool operator==( std::nullptr_t ) const;
    constexpr bool operator!=( std::nullptr_t ) const;

    TRef< Type > &operator=( std::nullptr_t );
    TRef< Type > &operator=( TRef &&other ) noexcept;
    TRef< Type > &operator=( const TRef &other );

private:
	void Acquire();
	void Release();

private:
	Type *ptr;
};

//--------------------------------------------------------------------------------------------------------------------------------
template< class Type >
TRef< Type >::TRef()
	: ptr( nullptr )
{
}

template< class Type >
TRef< Type >::TRef( std::nullptr_t )
	: ptr( nullptr )
{
}

template< class Type >
TRef< Type >::TRef( Type *_ptr )
	: ptr( _ptr )
{
}

template< class Type >
TRef< Type >::TRef( TRef &&other ) noexcept
	: ptr( other.ptr )
{
	other.ptr = nullptr;
}

template< class Type >
TRef< Type >::TRef( const TRef &other )
	: ptr( other.ptr )
{
	Acquire();
}

template< class Type >
TRef< Type >::~TRef()
{
	Release();
}

template< class Type >
constexpr bool TRef< Type >::IsValid() const
{
	return ptr != nullptr;
}

template< class Type >
const Type *TRef< Type >::GetPtr() const
{
	return ptr;
}

template< class Type >
Type *TRef< Type >::GetPtr()
{
	return ptr;
}

template< class Type >
const Type *TRef< Type >::operator->() const
{
	return ptr;
}

template< class Type >
Type *TRef< Type >::operator->()
{
	return ptr;
}

template< class Type >
const Type &TRef< Type >::operator*() const
{
	return *ptr;
}

template< class Type >
Type &TRef< Type >::operator*()
{
	return *ptr;
}

template< class Type >
constexpr bool TRef< Type >::operator==( const TRef &other ) const
{
	return ptr == other.ptr;
}

template< class Type >
constexpr bool TRef< Type >::operator!=( const TRef &other ) const
{
	return !operator==( other );
}

template< class Type >
constexpr bool TRef< Type >::operator==( std::nullptr_t ) const
{
	return ptr == nullptr;
}

template< class Type >
constexpr bool TRef< Type >::operator!=( std::nullptr_t ) const
{
	return ptr != nullptr;
}

template< class Type >
TRef< Type > &TRef< Type >::operator=( std::nullptr_t )
{
	Release();
    
    ptr = nullptr;
    
    return *this;
}

template< class Type >
TRef< Type > &TRef< Type >::operator=( TRef &&other ) noexcept
{
	if ( this == &other ) {
		return *this;
	}

	Release();

    ptr = other.ptr;
    other.ptr = nullptr;

	return *this;
}

template< class Type >
TRef< Type > &TRef< Type >::operator=( const TRef &other )
{
	if ( this == &other ) {
		return *this;
	}

	Release();

    ptr = other.ptr;
    Acquire();

	return *this;
}

template< class Type >
ALWAYS_INLINE void TRef< Type >::Acquire()
{
	if ( ptr ) {
		ptr->Acquire();
	}
}

template< class Type >
ALWAYS_INLINE void TRef< Type >::Release()
{
	if ( ptr ) {
		ptr->Release();

		if ( ptr->GetRef() == 0 ) {
			delete ptr;
			ptr = nullptr;
		}
	}
}
