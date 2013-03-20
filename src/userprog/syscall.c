#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "process.h"
#include <string.h>
#include "devices/shutdown.h"

int get_arg(struct intr_frame * f, int index);
int valid_arg(struct intr_frame * f, int index);
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

  /* check f->frame_pointer validity */
  struct thread *t = thread_current();

  if( f->frame_pointer != NULL && is_user_vaddr( f->frame_pointer ) && valid_arg(f, 0) )
  {
    memcpy( &syscall_num, f->esp, sizeof(int));
    switch( syscall_num )
    {
      case SYS_HALT:
        shutdown_power_off();
        break;

      case SYS_EXIT:
        if (! valid_arg(f, 1))
	{
	  // if arg is invalid, exit with -1
	  t->return_status = -1;
	  f->eax = -1;
          thread_exit();  
	}
	else
	{
	  // arg is valid, return that value
          t->return_status = get_arg(f, 1);
	  f->eax = get_arg(f, 1);
	  thread_exit();
	}
	break;

      case SYS_EXEC:
        if (! valid_arg(f, 1))
	{
	  // if arg is invalid, pid = -1
	  f->eax = -1;
          thread_exit();
	}
	else
	{
          // eax keeps the return value
          f->eax = process_execute((char *)get_arg(f, 1));

	}
	break;
      
      case SYS_WAIT:
        if (! valid_arg(f, 1))
	{
	  // if arg is invalid, pid = -1
	  f->eax = -1;
          thread_exit();  
	}
	else
	{
	  f->eax = process_wait((tid_t)get_arg(f, 1));
	}
	break;

      case SYS_WRITE:
        if (! valid_arg(f, 2) || ! valid_arg(f, 1) || ! valid_arg(f, 3))
	{
	  f->eax = -1;
          thread_exit();  
	}else
	{
	  putbuf(get_arg(f, 2), get_arg(f, 3));
	}
	break;

      default:
        // TODO
	;
    }
  }
  else
  {
    thread_exit ();
  }
}

/*
Check validity of ith argument.
They may exit after calling this.
*/
int
valid_arg(struct intr_frame * f, int index)
{
  int *ptr = (int *)(f->esp) + index; // current point (to stack)
  struct thread *t = thread_current();
  
  return ptr != NULL && is_user_vaddr( ptr ) && ( pagedir_get_page(t->pagedir, ptr) != NULL );
}

/* get the ith argument above esp */
int
get_arg(struct intr_frame * f, int index)
{
  return * ( (int *)(f->esp) + index ); // current point (to stack)
}
