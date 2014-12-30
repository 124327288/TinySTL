#ifndef _SUFFIX_ARRAY_H_
#define _SUFFIX_ARRAY_H_

#include <vector>
#include <memory>

namespace TinySTL{
	
	class suffix_array{
	private:
		//typedef std::unique_ptr<std::vector<int>> vecPtr;
	private:
		std::vector<int> _array;
	public:
		template<class InputIterator>
		//arr - Դ����
		//len - Դ���鳤��
		//max_len - max_len�����ַ������ַ���ȡֵ��Χ���ǻ��������һ��������
		//          ���ԭ���ж�����ĸ����ֱ��ȡ128�����ԭ���б����������Ļ�����m����ȡ������������1��ֵ��
		suffix_array(InputIterator arr, int len, int max_len = 128){
			//�����������������ʵ�ֵı����㷨
			_array.resize(len);
			int wa[1024], wb[1024], wv[1024], ws[1024];

			int i, j, p, *x = wa, *y = wb, *t;
			for (i = 0; i < max_len; i++) ws[i] = 0;
			for (i = 0; i < len; i++) ws[x[i] = arr[i]]++;
			for (i = 1; i < max_len; i++) ws[i] += ws[i - 1];
			for (i = len - 1; i >= 0; i--) _array[--ws[x[i]]] = i;
			for (j = 1, p = 1; p < len; j *= 2, max_len = p)
			{
				for (p = 0, i = len - j; i < len; i++) y[p++] = i;
				for (i = 0; i < len; i++) if (_array[i] >= j) y[p++] = _array[i] - j;
				for (i = 0; i < len; i++) wv[i] = x[y[i]];
				for (i = 0; i < max_len; i++) ws[i] = 0;
				for (i = 0; i < len; i++) ws[wv[i]]++;
				for (i = 1; i < max_len; i++) ws[i] += ws[i - 1];
				for (i = len - 1; i >= 0; i--) _array[--ws[wv[i]]] = y[i];
				for (t = x, x = y, y = t, p = 1, x[_array[0]] = 0, i = 1; i < len; i++)
					x[_array[i]] = cmp(y, _array[i - 1], _array[i], j) ? p - 1 : p++;
			}
			return;
		}

		const std::vector<int> suffixArray()const{
			return _array;
		}
	private:
		bool cmp(int *arr, int a, int b, int l){
			return arr[a] == arr[b] && arr[a + l] == arr[b + l];
		}
	};
}

#endif