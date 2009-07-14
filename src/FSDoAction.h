/*
 *  FSDoAction.h
 *  Transform SWF
 *
 * Copyright (c) 2001-2004 Flagstone Software Ltd.. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of Flagstone Software Ltd. nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __FSDoAction_h__
#define __FSDoAction_h__ 1

#include "FSMovieObject.h"

#include "FSVector.h"
#include "FSActionObject.h"

#ifdef WIN32
#ifdef _WINDLL
DLL_TEMPLATE template class DLL_DECL transform::FSVector<transform::FSActionObject*>;
#endif // _WINDLL
#endif // WIN32

namespace transform
{
/**
 * %FSDoAction is used to specify a sequence of actions that are executed when a frame is displayed.
 *
<table class="datasheet">

<tr><th align="left" colspan="2">Attributes</th></tr>

<tr><td><a name="FSDoAction_0">type</a></td>
<td>Identifies the data structure when it is encoded. Read-only.</td>
</tr>

<tr><td><a name="FSDoAction_1">actions</a></td>
<td>An array of FSActionObjects which are executed by the Flash Player when the current frame is displayed. The actions are executed in the order they appear in the array.</td>
</tr>

<tr><td><a name="FSDoAction_2">encodedActions</a></td>
<td>An array of bytes containing encoded actions can also be set. The encoded actions are typically generated by the parser in the Translate framework. The actions array and encodedActions cannot both be valid at the same time. Accessor methods used to set either of the attributes will set the other to null.</td>
</tr>

</table>

<p>To define the actions for a given frame the FSDoAction object should be added to a movie after the previous frame is displayed but before the FSShowFrame object that displays the 'current' frame and triggers the actions to be executed.</p>

<p>Only one FSDoAction object can be used to specify the actions for a given frame. If more than one FSDoAction object is added in a single frame only the actions contained in the last FSDoAction object (before the FSShowFrame object) will be executed when the frame is displayed. The other FSDoAction objects will be ignored.</p>

<h1 class="datasheet">Examples</h1>

<pre>
FSVector&gt;FSActionObject*&gt; actions;

actions.push_back(new FSPush("_root"));
actions.push_back(new FSAction(FSAction::GetVariable));
...

movie.push_back(new FSShowFrame()); // previous frame
...

movie.push_back(new FSDoAction(actions));
movie.push_back(new FSShowFrame()); // frame where actions will be executed
</pre>

<h1 class="datasheet">History</h1>

<p>The FSDoAction class represents the DoAction tag from the Macromedia Flash (SWF) File Format Specification. It was introduced in Flash 1.</p>

 */
    class DLL_DECL FSDoAction : public FSMovieObject
    {
public:
        FSDoAction(FSInputStream* aStream);

        /**  Constructs an FSDoAction object with an array actions. The FSDoAction object assumes ownership of all the action objects in the array.

            @param anArray the array of action objects.
            */
        FSDoAction(const FSVector<FSActionObject*>& anArray) : FSMovieObject(DoAction), actions(anArray), encodedActions(0), encodedLength(0) {}

        /**
         * Constructs an FSDoAction object with an array of encoded actions
         * generated by the classes in Translate.
         *
         * @param bytes an array of encoded action objects.
         * @param size the number of bytes in the encoded data.
         */
        FSDoAction(byte* bytes, size_t size);

        FSDoAction(const FSDoAction& rhs);

        ~FSDoAction();

        FSDoAction& operator= (const FSDoAction& rhs);

        FSDoAction* clone() const;

        const char* className() const;

        /** Adds an action to the array of actions. The FSDoAction object assumes ownership of the action object.

            @param anAction a pointer to an action object.
            */
        void add(FSActionObject* anAction) { actions.push_back(anAction); }

        /** Adds all the actions in an array to the array of actions. The FSDoAction object assumes ownership of all the action objects in the array.

            @param anArray an array of pointers to action objects.
            */
        void add(const FSVector<FSActionObject*>& anArray);

        /** Get the array of actions that are executed when the frame is displayed.

            @return the array of action objects.
            */
        FSVector<FSActionObject*>& getActions() { return actions; }

        /** Set the array of actions that will be executed when the next ShowFrame tag is executed by the Flash Player. The FSDoAction object assumes ownership of all the action objects in the array.

            @param anArray the array of action objects.
            */
        void setActions(const FSVector<FSActionObject*>& anArray);

        /**
         * Set the array of encoded actions generated by the classes in the Translate
         * framework. If the object already contains an array of actions then they
         * will be deleted.
         *
         * @param bytes the array of encoded actions.
         * @param size the number of bytes in the encoded data.
         */
        void setEncodedActions(byte* bytes, size_t size);

        int lengthInStream(FSOutputStream* aStream);
        void encodeToStream(FSOutputStream* aStream);
        void decodeFromStream(FSInputStream* aStream);

private:
        FSVector<FSActionObject*> actions;
        byte* encodedActions;
        size_t encodedLength;
    };
}

#endif