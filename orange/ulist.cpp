#include "ulist.h"
#include "OGEventDispatcher.h"


OG_BEGIN



//uPNode FindCallback(uPNode ptr, void *target)
//{
//	uPNode ret = ptr;
//	while (ret != nullptr)
//	{
//		if (target == ret->target)
//		{
//			return ret;
//		}
//		ret = ret->next;
//	}
//	return nullptr;
//}
//
//void addCallback(uPNode *head, void *target, callFunc callback)
//{
//	if (!FindCallback(*head, target))
//	{
//		//�����¡��ص����ڵ�
//		uPNode curNode = new uNode;
//		curNode->target = target;
//		curNode->callback = callback;
//
//		//����ǲ���ͷ�ڵ㣬���ʾ��������
//		if ((*head) == nullptr)
//		{//���û��������ֱ�����׽ڵ㸳ֵ��ǰ���γ�һ���ջ�����ָ���׽ڵ㣬
//			(*head) = curNode;
//			(*head)->prev = (*head);
//
//			(*head)->next = nullptr;
//		}
//		else {
//
//			//��ǰǰ��ָ���׽ڵ��ǰ�����ڻ��������о���β��
//			curNode->prev = (*head)->prev;
//			//�׽ڵ���ָ���β�ڵ�ĺ�̽ڵ�ָ���·���Ľڵ�
//			(*head)->prev->next = curNode;
//			//����β�ڵ�Ϊ��ǰ
//			(*head)->prev = curNode;
//			//nextû�бջ�������Ϊ��
//			curNode->next = nullptr;
//		}
//	}
//}
//
//void delCallback(uPNode *head, void *target)
//{
//	uPNode del = nullptr;
//	if ((del = FindCallback(*head, target)) != nullptr)
//	{
//		//������ҵ���Ҫɾ����Ŀ��del
//
//		assert((del)->prev != NULL);
//		if ((del)->prev == (del)) {
//			//ֻ��һ���ڵ㣬�ÿ�
//			(*head) = NULL;
//		}
//		else if ((del) == (*head)) {
//			//���ҵ���Ϊ�׽ڵ㣬������del��ǰ���ڵ����һ���ڵ㣬del������Ҫɾ����
//			//�׽ڵ���¸��ڵ��ǰ������ָ��β�ڵ㣬
//			(del)->next->prev = (del)->prev;
//			//headǰ��һ��
//			(*head) = (del)->next;
//		}
//		else {
//			//���ҵ����������ڵ�λ�ã�delǰ���ڵ�ָ��del�ĺ�̽ڵ�
//			(del)->prev->next = (del)->next;
//			
//			//del�Ƿ������Ľڵ�(->next == null��ʾ�����ڵ�)
//			if ((del)->next) {
//				//�������β�ڵ㣬���del���¸��ڵ�����del���ϸ��ڵ�
//				(del)->next->prev = (del)->prev;
//			}
//			else {
//				//�����β�ڵ㣬���ͷ�ڵ��ǰ��ָ��del��ǰ������ǰ���γ�һ���ջ�
//				(*head)->prev = (del)->prev;
//			}
//		}
//
//
//		delete del;
//
//	}
//}
//
// void TraverseUpdateList(uPNode node, float delta)
// {
// 	if (nullptr != node)
// 	{
// 		uPNode(curNode) = (node), tmp = nullptr;
// 		//���ж�ʱ���ͻ�ȡnext�����ݣ���Ϊ���µ���̽ڵ������
// 		while ((curNode) && (tmp = curNode->next, 1 ))
// 		{
// 			curNode->callback(delta);
// 
// 			(curNode) = tmp;
// 		}
// 	}
// 
// }
// 
// 
// 
// uPEventNode FindCallback(uPEventNode ptr, EventDispatcher * target)
// {
// 	return nullptr;
// }
// 
// void addCallback(uPEventNode * head, EventDispatcher * target)
// {
// }
// 
// void delCallback(uPEventNode * head, EventDispatcher * target)
// {
// }
// 
// void TraverseEventList(uPEventNode node)
// {
// }



OG_END
