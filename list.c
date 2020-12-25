#include "all.h"

// 创建头节点
list list_init()
{
	list head = (list)malloc(sizeof(struct node));
	
	if(head == NULL) // 空间分配不成功则报错
		_ERROR_H;
	
	head->prev = head->next = head;
	
	return head;
}

// 创建新节点
list list_new(char *picname, int *num)
{
	list new = (list)malloc(sizeof(struct node));
	
	if(new == NULL) // 空间分配不成功则报错
		_ERROR_H;
	
	new->picname = picname;
	new->number = *num;
	
	return new;
}

// 头插法
list list_add_top(list head, list new, char *picname)
{
	if(head == NULL || new == NULL)
		return NULL;
	
	new->prev = head;
	new->next = head->next;
	
	new->prev->next = new;
	new->next->prev = new;
}

// 尾插法
list list_add_tail(list head, list new)
{
	if(head == NULL || new == NULL)
		return NULL;
	
	list p = head;
	
	while(p->next != head) // 找到最后一个节点
	{
		p = p->next; // 不是最后一个节点则执行
	}
	
	new->prev = p;
	new->next = p->next;
	
	new->prev->next = new;
	new->next->prev = new;
}

// 后插法
list insert_next(list anchor, list new)
{
	if(anchor == NULL || new == NULL)
		return NULL; 
	
	new->prev = anchor;
	new->next = anchor->next;
	
	new->prev->next = new;
	new->next->prev = new;
}

// 前插法
list insert_prev(list anchor, list new)
{
	if(anchor == NULL || new == NULL)
		return NULL;
	
	new->prev = anchor->prev;
	new->next = anchor;
	
	new->prev->next = new;
	new->next->prev = new;
}

// 遍历链表
list list_show(list head)
{
	if(head == NULL)
		return NULL;
	
	list p = head;
	
	while(p->next != head) // 找到最后一个节点
	{
		p = p->next; // 不是最后一个节点则执行
		printf("picname is %s\n", p->picname);
	}
	
	return NULL;
}

// 遍历文件夹并将bmp文件存入链表
list bmpstat(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage: %s <path>\n", argv[0]);
	}
	
	// 将文件argv[1]的属性信息存储在info中
	struct stat info;
	stat(argv[1], &info);
	
	// 创建链表头节点
	list head = list_init();
	list p = head;
	
	int bmp_h = 0;
	int bmp_w = 0;
	int num = 0;
	// 1：如果argc[1]是一个目录，则需打印该目录下所有文件的相关信息   “并存入链表”
	if(S_ISDIR(info.st_mode))
	{
		DIR *dp = opendir(argv[1]); // dp为目录指针
		if(dp == NULL)
		{
				perror("open is failed\n");
		}
		struct dirent *ep;
		//chdir(argv[1]);  // 切换工作路径，进入argv[1]中，好像不需要。readdir就行了。
		
		// 迭代获取所有的目录项，并打印他们的类型、权限和名字
		while(1)
		{
			ep = readdir(dp); // ep为目录项指针 这指针会在目录项被读取后往后移？（和文本读取一样）
			if(ep == NULL)
				break;
			//printf("%s\n", ep->d_name);
			
			if(ep->d_name[0] == '.') // 忽略以'.'开头的文件
				continue;
			
			// 只打印后缀名.bmp且 bmp_w<=600 && bmp_h<=360 的文件
			if(strstr(ep->d_name, ".bmp"))
			{
				pic_size(ep->d_name, &bmp_w, &bmp_h);
				if( bmp_w<=600 && bmp_h<=360 )
				{
					list new = list_new(ep->d_name, &num);
					insert_next(p, new);
					p = new;
					num++;
				
					printf("\t%s\n", ep->d_name);
				}
			}

			//printf("\t%s\n", ep->d_name);
			
		}
	}
	// 2：如果argv[1]是一个普通文件，则直接打印其相关属性信息
	else
	{
		printf("\t%s\n", argv[1]);
	}
	
	list_show(head);
	return head;
}

// 摧毁链表
int list_destory(list head)
{
	list p = head;
	if(head->next != head)
	{
		p = head->next;
		head->next = p->next;
		free(p);
	}
	else
	{
		free(head);
	}
	return 0;
}




