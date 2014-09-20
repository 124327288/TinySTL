#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <type_traits>

#include "Allocator.h"
#include "Iterator.h"
#include "UninitializedFunctions.h"

namespace TinySTL{

	namespace {
		template<class T>
		class viter: public iterator<random_access_iterator<T, ptrdiff_t>, T>{
		private:
			T * ptr_;
		public:
			viter() :ptr_(0){}
			explicit viter(T *ptr):ptr_(ptr){}
			viter(const viter& vit);
			viter& operator = (const viter& vit);

			T& operator *(){ return *ptr_; }
			T *operator ->(){ return &(operator *()); }
			
			viter& operator ++(){ ++ptr_; return *this; }
			viter operator ++(int){ viter temp = *this; ++(*this); return temp; }
			viter& operator --(){ --ptr_; return *this; }
			viter operator --(int){ viter temp = *this; --(*this); return temp; }

			bool operator == (const viter& vit){ return ptr_ == vit.ptr_; }
			bool operator != (const viter& vit){ return !(*this == vit); }
			typename viter::difference_type operator - (const viter& vit){ return ptr_ - vit.ptr_; }

			friend typename viter::difference_type operator - (const viter& left, const viter& right);
		};
		template<class T>
		viter<T>::viter(const viter& vit){
				ptr_ = vit.ptr_;
		}
		template<class T>
		viter<T>& viter<T>::operator = (const viter& vit){
			if (this != &vit){
				ptr_ = vit.ptr_;
			}
		}
		template<class T>
		typename viter<T>::difference_type operator - (const viter<T>& left, const viter<T>& right){
			return left - right;
		}
	}// end of anonymous namespace

	template<class T, class Alloc = allocator<T>>
	class vector{
	private:
		T *start_;
		T *finish_;
		T *endOfStorage_;

		typedef Alloc dataAllocator;
		//Alloc dataAllocator;
	public:
		typedef T			value_type;
		typedef viter<T>	iterator;
		typedef iterator	pointer;
		typedef T&			reference;
		typedef size_t		size_type;
		typedef typename iterator::difference_type difference_type;
	public:
		//���죬���ƣ�������غ���
		vector()
			:start_(0), finish_(0), endOfStorage_(0){}
		explicit vector(const size_type n);
		vector(const size_type n, const value_type& value);
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);
		vector(const vector& v);
		vector(vector&& v);
		vector& operator = (const vector& v);
		~vector(){ 
			dataAllocator::destroy(start_, finish_); 
			dataAllocator::deallocate(start_, endOfStorage_ - start_); 
		}

		//���������
		iterator begin(){ return iterator(start_); }
		iterator end(){ return iterator(finish_); }

		//�޸�������صĲ���
		void clear(){
			dataAllocator::destroy(start_, finish_);
			finish_ = start_;
		}
	private:
		void allocateAndFillN(const size_type n, const value_type& value){
			start_ = dataAllocator::allocate(n);
			uninitialized_fill_n(start_, n, value);
			finish_ = endOfStorage_ = start_ + n;
		}
		template<class InputIterator>
		void allocateAndCopy(InputIterator first, InputIterator last){
			start_ = dataAllocator::allocate(last - first);
			finish_ = uninitialized_copy(first, last, start_);
			endOfStorage_ = finish_;
		}

		template<class InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type){
			allocateAndCopy(first, last);
		}
		template<class Integer>
		void vector_aux(Integer n, Integer value, std::true_type){
			allocateAndFillN(n, value);
		}
	};
	//***********************���죬���ƣ��������***********************
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n){
		allocateAndFillN(n, value_type());
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value){
		allocateAndFillN(n, value);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	vector<T, Alloc>::vector(InputIterator first, InputIterator last){
		//����ָ������ּ������ĺ���
		vector_aux(first, last, typename std::is_integral<InputIterator>::type());
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const vector& v){
		allocateAndCopy(v.start_, v.finish_);
	}
	template<class T, class Alloc>
	vector<T, Alloc>::vector(vector&& v){
		start_ = v.start_;
		finish_ = v.finish_;
		endOfStorage_ = v.endOfStorage_;
		v.clear();
	}
	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator = (const vector& v){
		if (this != &v){
			allocateAndCopy(v.start_, v.finish_);
		}
		return *this;
	}
}

#endif