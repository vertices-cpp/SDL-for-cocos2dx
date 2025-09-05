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
//		//创建新“回调”节点
//		uPNode curNode = new uNode;
//		curNode->target = target;
//		curNode->callback = callback;
//
//		//如果是不是头节点，则表示已有内容
//		if ((*head) == nullptr)
//		{//如果没有内容则直接用首节点赋值。前驱形成一个闭环，并指向首节点，
//			(*head) = curNode;
//			(*head)->prev = (*head);
//
//			(*head)->next = nullptr;
//		}
//		else {
//
//			//当前前驱指向首节点的前驱，在环形链表中就是尾部
//			curNode->prev = (*head)->prev;
//			//首节点所指向的尾节点的后继节点指向新分配的节点
//			(*head)->prev->next = curNode;
//			//更新尾节点为当前
//			(*head)->prev = curNode;
//			//next没有闭环，保持为空
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
//		//如果查找到需要删除的目标del
//
//		assert((del)->prev != NULL);
//		if ((del)->prev == (del)) {
//			//只有一个节点，置空
//			(*head) = NULL;
//		}
//		else if ((del) == (*head)) {
//			//查找到的为首节点，则连接del的前个节点与后一个节点，del部分需要删除，
//			//首节点的下个节点的前驱依旧指向尾节点，
//			(del)->next->prev = (del)->prev;
//			//head前进一步
//			(*head) = (del)->next;
//		}
//		else {
//			//查找到的在其它节点位置，del前方节点指向del的后继节点
//			(del)->prev->next = (del)->next;
//			
//			//del是否是最后的节点(->next == null表示是最后节点)
//			if ((del)->next) {
//				//如果不是尾节点，则把del的下个节点连到del的上个节点
//				(del)->next->prev = (del)->prev;
//			}
//			else {
//				//如果是尾节点，则把头节点的前驱指向del的前驱，由前驱形成一个闭环
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
// 		//在判断时，就获取next的内容，作为更新到后继节点的依据
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
