Keil C51�е�printf()���׼��C���printf()�������в�ͬ������Ӧ�İ����ĵ���������������
The optional characters l or L may immediately precede the type character to respectively specify long types for d, i, u, o, x, and X.
The optional characters b or B may immediately precede the type character to respectively specify char types for d, i, u, o, x, and X.
Ҳ����˵��ʹ��C51��printf()������ӡ%d/i/u/o/x/X��ʽʱ�������Ҫָ���ñ����Ĵ洢��ʽl/L/b/B��
������ı���cΪchar���ͣ���˿ɽ���Ӧ�Ĵ����Ϊ���£�
printf("%bd\n",c);
printf("%bu\n",c);
printf("%bx\n",c);
���ɵõ���ȷ�Ľ����
��
������cΪuint16ʱ�����轫%bd�ȸ�Ϊ%hd��(�����C51�����ĵ�����û�еģ������Լ��ܽ������)
������cΪuint32ʱ�����轫%bd�ȸ�Ϊ%ld��
ϣ���ܰ������㡣


��ӡ������c%�����ڴ�����ȡ����һ���ֽڵ�ֵ,û������,����һ����ӡ%d,%y,%x�����ı�����ʱ��,���ڴ�����ȡ������c�ı����洢��ַΪ��ʼ��ַ,
�������ֽ�Ϊ���ݵ�Ԫ,ȡ������ֵ,Ȼ������ΪcΪ��ַ�ĺ�һ����ַ��Ԫ��������0,����,ȡ������ֵ��256�ı���.