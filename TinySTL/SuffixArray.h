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
		//max_len - max_len�����ַ���arr���ַ���ȡֵ��Χ���ǻ��������һ��������
		//          ���ԭ���ж�����ĸ����ֱ��ȡ128�����ԭ���б����������Ļ�����m����ȡ������������1��ֵ��
		suffix_array(InputIterator arr, size_t len, size_t max_len = 128){
			//�����������������ʵ�ֵı����㷨
			//�㷨ʱ�临�Ӷ� = O(nlg(n))
			_array.resize(len);
			int wa[1024], wb[1024], wv[1024], ws[1024];
			int i, j, p, *x = wa, *y = wb, *t;

			//�������д����ǰѸ����ַ���Ҳ������Ϊ1���ַ��������л�������
			for (i = 0; i < max_len; i++) ws[i] = 0;
			//x[]���汾���Ǳ��������׺��rankֵ�ģ��������ﲢû��ȥ�洢rankֵ����Ϊ����ֻ���漰x[]�ıȽϹ����������һ�����Բ��ô洢��ʵ��rankֵ���ܹ���ӳ��ԵĴ�С���ɡ�
			for (i = 0; i < len; i++) ws[x[i] = arr[i]]++;
			for (i = 1; i < max_len; i++) ws[i] += ws[i - 1];
			//i֮���Դ�len-1��ʼѭ������Ϊ�˱�֤�ڵ��ַ���������ȵ��ַ���ʱ��Ĭ�Ͽ�ǰ���ַ�����СһЩ��
			for (i = len - 1; i >= 0; i--) _array[--ws[x[i]]] = i;

			//�������ѭ����p����rankֵ���õ��ַ��������������p�ﵽlen����ô�����ַ����Ĵ�С��ϵ���Ѿ������ˡ�
			//j����ǰ���ϲ����ַ����ĳ��ȣ�ÿ�ν���������Ϊj���ַ����ϲ���һ������Ϊ2*j���ַ�������Ȼ��������ַ���ĩβ��������ֵӦ�����ۣ���˼����һ���ġ�
			//max_lenͬ��������������Ԫ�ص�ȡֵ��Χ
			for (j = 1, p = 1; p < len; j *= 2, max_len = p)
			{
				//�������д���ʵ���˶Եڶ��ؼ��ֵ�����
				for (p = 0, i = len - j; i < len; i++) y[p++] = i;
				for (i = 0; i < len; i++) if (_array[i] >= j) y[p++] = _array[i] - j;
				//�ڶ��ؼ��ֻ���������ɺ�y[]���ŵ��ǰ��ڶ��ؼ���������ַ����±�
				//�����൱����ȡ��ÿ���ַ����ĵ�һ�ؼ��֣�ǰ��˵����x[]�Ǳ���rankֵ�ģ�Ҳ�����ַ����ĵ�һ�ؼ��֣����ŵ�wv[]�����Ƿ�������ʹ��
				//�������д����ǰ���һ�ؼ��ֽ��еĻ�������
				for (i = 0; i < len; i++) wv[i] = x[y[i]];
				for (i = 0; i < max_len; i++) ws[i] = 0;
				for (i = 0; i < len; i++) ws[wv[i]]++;
				for (i = 1; i < max_len; i++) ws[i] += ws[i - 1];
				for (i = len - 1; i >= 0; i--) _array[--ws[wv[i]]] = y[i];
				//�������о��Ǽ���ϲ�֮���rankֵ�ˣ����ϲ�֮���rankֵӦ�ô���x[]���棬�����Ǽ����ʱ���ֱ����õ���һ���rankֵ��Ҳ��������x[]����ŵĶ���������Ҽ�Ҫ��x[]�����ã���Ҫ��x[]����ţ���ô�죿
				//��Ȼ���Ȱ�x[]�Ķ����ŵ�����һ���������棬ʡ�����ˡ���������ý���ָ��ķ�ʽ����Чʵ���˽�x[]�Ķ��������ơ�����y[]�С�
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