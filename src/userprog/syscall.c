#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int syscall_num;
  int syscall_arg;

  printf ("system call!\n");

  /* check f->frame_pointer validation */
  /*
  struct thread *t = thread_current();

  if( f->frame_pointer != NULL && is_user_vaddr( f->frame_pointer ) 
  	&& f->esp !=NULL && is_user_vaddr( f->esp ) && ( pagedir_get_page(t->pagedir, f->esp) != NULL ) )
  {
    memcpy( &syscall_num, f->esp, sizeof(int));
    switch( syscall_num )
    {
      case SYS_EXIT:
        t->return_status = get_arg(f, 1);
	printf("%s: exit(%d)\n", t->name, t->return_status );
	thread_exit();
	break;
    }
  }
  else
  {
    thread_exit ();
  }
  */

  thread_exit();
}

/* get the ith argument above esp */
/*
int
get_arg(struct intr_frame * f, int index)
{
  int *ptr = (int *)(f->esp) + index;
  if( ptr >= PHYS_BASE )
  {
    t->return_status = -1;
    printf("%s: exit(%d)\n", t->name, t->return_status );
    thread_exit();
  }
  return *ptr;
}
*/
