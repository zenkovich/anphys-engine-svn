/*
 * pthread_once.c
 *
 * Description:
 * This translation unit implements miscellaneous thread functions.
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads-win32 - POSIX Threads Library for Win32
 *      Copyright(C) 1998 John E. Bossom
 *      Copyright(C) 1999,2005 Pthreads-win32 contributors
 * 
 *      Contact Email: rpj@callisto.canberra.edu.au
 * 
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *      http://sources.redhat.com/pthreads-win32/contributors.html
 * 
 *      This library is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU Lesser General Public
 *      License as published by the Free Software Foundation; either
 *      version 2 of the License, or (at your option) any later version.
 * 
 *      This library is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *      Lesser General Public License for more details.
 * 
 *      You should have received a copy of the GNU Lesser General Public
 *      License along with this library in the file COPYING.LIB;
 *      if not, write to the Free Software Foundation, Inc.,
 *      59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "pthread.h"
#include "implement.h"


static void PTW32_CDECL
ptw32_once_on_init_cancel (void * arg)
{
  /* when the initting thread is cancelled we have to release the lock */
  ptw32_mcs_local_node_t *node = (ptw32_mcs_local_node_t *)arg;
  ptw32_mcs_lock_release(node);
}

int
pthread_once (pthread_once_t * once_control, void (*init_routine) (void))
{
  if (once_control == NULL || init_routine == NULL)
    {
      return EINVAL;
    }
  
  if (!InterlockedExchangeAdd((LPLONG)&once_control->done, 0)) /* MBR fence */
    {
      ptw32_mcs_local_node_t node;

      ptw32_mcs_lock_acquire((ptw32_mcs_lock_t *)&once_control->lock, &node);

      if (!once_control->done)
	{

#ifdef _MSC_VER
#pragma inline_depth(0)
#endif

	  pthread_cleanup_push(ptw32_once_on_init_cancel, (void *)&node);
	  (*init_routine)();
	  pthread_cleanup_pop(0);

#ifdef _MSC_VER
#pragma inline_depth()
#endif

	  once_control->done = PTW32_TRUE;
	}

	ptw32_mcs_lock_release(&node);
    }

  return 0;

}				/* pthread_once */
