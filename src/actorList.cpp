/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 
 <TODO: Add in GPLv2-notice, need to make sure if we are v2-only, or v2-or-later,
 we are atleast v2>
 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common.h"

int sortCompareFunction(const void* param1, const void* param2)
{
	int distance1 = 0;
	int distance2 = 0;
	actorStruct* actor1Ptr;
	actorStruct* actor2Ptr;
	ZVStruct* actor1ZvPtr;
	ZVStruct* actor2ZvPtr;
	ZVStruct localZv1;
	ZVStruct localZv2;
	int flag = 0;
	int y1;
	int y2;
	
	ASSERT(*(int*)param1 >=0 && *(int*)param1 < NUM_MAX_ACTOR);
	ASSERT(*(int*)param2 >=0 && *(int*)param2 < NUM_MAX_ACTOR);
	
	actor1Ptr = &actorTable[*(int*)param1];
	actor2Ptr = &actorTable[*(int*)param2];
	
	actor1ZvPtr = &actor1Ptr->zv;
	actor2ZvPtr = &actor2Ptr->zv;
	
	if(actor1Ptr->room != currentDisplayedRoom)
	{
		copyZv(actor1ZvPtr, &localZv1);
		getZvRelativePosition(&localZv1, actor1Ptr->room, currentDisplayedRoom);
		actor1ZvPtr = &localZv1;
	}
	
	if(actor2Ptr->room != currentDisplayedRoom)
	{
		copyZv(actor2ZvPtr, &localZv2);
		getZvRelativePosition(&localZv2, actor2Ptr->room, currentDisplayedRoom);
		actor2ZvPtr = &localZv2;
	}
	
	y1 = ((((actor1ZvPtr->ZVY1 + actor1ZvPtr->ZVY2) / 2) - 2000) / 2000) * 2000;
	y2 = ((((actor2ZvPtr->ZVY1 + actor2ZvPtr->ZVY2) / 2) - 2000) / 2000) * 2000;
	
	if(y1 == y2) // both y in the same range
	{
		if(
		   ((actor1ZvPtr->ZVX1 > actor2ZvPtr->ZVX1) && (actor2ZvPtr->ZVX2 > actor1ZvPtr->ZVX1)) ||
		   ((actor1ZvPtr->ZVX2 > actor2ZvPtr->ZVX1) && (actor1ZvPtr->ZVX2 < actor2ZvPtr->ZVX2)) ||
		   ((actor2ZvPtr->ZVX1 > actor1ZvPtr->ZVX1) && (actor1ZvPtr->ZVX2 > actor2ZvPtr->ZVX1)) ||
		   ((actor2ZvPtr->ZVX2 > actor1ZvPtr->ZVX1) && (actor2ZvPtr->ZVX2 < actor1ZvPtr->ZVX2)) )
		{
			flag |= 1;
		}
		
		if(
		   (actor1ZvPtr->ZVZ1 > actor2ZvPtr->ZVZ1 && actor1ZvPtr->ZVZ1 < actor2ZvPtr->ZVZ2) ||
		   (actor1ZvPtr->ZVZ2 > actor2ZvPtr->ZVZ1 && actor1ZvPtr->ZVZ2 < actor2ZvPtr->ZVZ2) ||
		   (actor2ZvPtr->ZVZ1 > actor1ZvPtr->ZVZ1 && actor1ZvPtr->ZVZ2 > actor2ZvPtr->ZVZ1) ||
		   (actor2ZvPtr->ZVZ2 > actor1ZvPtr->ZVZ1 && actor2ZvPtr->ZVZ2 < actor1ZvPtr->ZVZ2) )
		{
			flag |= 2;
		}
		
		//TODO: remove hack and find the exact cause of the bug in the sorting algorithme
		flag = 0;
		
		if(flag == 0)
		{
			distance1 = computeDistanceToPoint(translateX,translateZ,(actor1ZvPtr->ZVX1+actor1ZvPtr->ZVX2)/2,(actor1ZvPtr->ZVZ1+actor1ZvPtr->ZVZ2)/2);
			distance2 = computeDistanceToPoint(translateX,translateZ,(actor2ZvPtr->ZVX1+actor2ZvPtr->ZVX2)/2,(actor2ZvPtr->ZVZ1+actor2ZvPtr->ZVZ2)/2);
		}
		else
		{
			if(flag & 2) // intersect on Z
			{
				if( abs(translateX - actor1ZvPtr->ZVX1) < abs(translateX - actor1ZvPtr->ZVX2) )
				{
					distance1 = translateX - actor1ZvPtr->ZVX1;
				}
				else
				{
					distance1 = translateX - actor1ZvPtr->ZVX2;
				}
				
				if( abs(translateX - actor2ZvPtr->ZVX1) < abs(translateX - actor2ZvPtr->ZVX2) )
				{
					distance2 = translateX - actor2ZvPtr->ZVX1;
				}
				else
				{
					distance2 = translateX - actor2ZvPtr->ZVX2;
				}
			}
			if(flag & 1) // intersect on X
			{
				if( abs(translateZ - actor1ZvPtr->ZVZ1) < abs(translateZ - actor1ZvPtr->ZVZ2) )
				{
					distance1 += translateZ - actor1ZvPtr->ZVZ1;
				}
				else
				{
					distance1 += translateZ - actor1ZvPtr->ZVZ2;
				}
				
				if( abs(translateZ - actor2ZvPtr->ZVZ1) < abs(translateZ - actor2ZvPtr->ZVZ2) )
				{
					distance2 += translateZ - actor2ZvPtr->ZVZ1;
				}
				else
				{
					distance2 += translateZ - actor2ZvPtr->ZVZ2;
				}
			}
		}
		
	}
	else
	{
		distance1 = abs(translateY - 2000 - y1);
		distance2 = abs(translateY - 2000 - y2);
	}
	
	if(distance1>distance2)
	{
		return(-1);
	}
	
	if(distance1<distance2)
	{
		return(1);
	}
	
	return(0);
}

void sortActorList()
{
	qsort(sortedActorTable, numActorInList, sizeof(int), sortCompareFunction);
}