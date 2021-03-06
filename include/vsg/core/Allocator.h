#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/core/Object.h>
#include <vsg/core/Visitor.h>
#include <vsg/core/ConstVisitor.h>
#include <vsg/core/ref_ptr.h>

#include <vsg/utils/stream.h>

#include <vsg/traversals/DispatchTraversal.h>
#include <vsg/traversals/CullTraversal.h>

namespace vsg
{

    class Allocator : public Object
    {
    public:
        std::size_t sizeofObject() const noexcept override { return sizeof(Allocator); }

        void accept(Visitor& visitor) override { visitor.apply(static_cast<Allocator&>(*this)); }
        void accept(ConstVisitor& visitor) const override { visitor.apply(static_cast<const Allocator&>(*this)); }
        void accept(DispatchTraversal& visitor) const override { visitor.apply(static_cast<const Allocator&>(*this)); }
        void accept(CullTraversal& visitor) const override { visitor.apply(static_cast<const Allocator&>(*this)); }

        virtual void* allocate(std::size_t n, const void* hint );

        virtual void* allocate(std::size_t size);

        virtual void deallocate(const void* ptr, std::size_t size=0);

        Auxiliary* getOrCreateSharedAuxiliary();

        void detachSharedAuxiliary(Auxiliary* auxiliary);

#if 1
        // likely to remove this create method, as vsg::Inherit::create(..) now provides same functionality.
        template<typename T, typename... Args>
        ref_ptr<T> create(Args... args)
        {
            // need to think about alignment...
            std::size_t size = sizeof(T);
            void* ptr = allocate(size);
            ref_ptr<T> object = new (ptr) T(args...);
            object->setAuxiliary(getOrCreateSharedAuxiliary());

            std::size_t new_size = object->sizeofObject();
            if (new_size != size)
            {
                throw make_string("Warning: Allocator::create(",typeid(T).name(),") mismatch sizeof() = ",size,", ",new_size);
            }
            return object;
        }
#endif

    protected:
        virtual ~Allocator();

        Auxiliary*  _sharedAuxiliary = nullptr;
        std::size_t _bytesAllocated = 0;
        std::size_t _countAllocated = 0;
        std::size_t _bytesDeallocated = 0;
        std::size_t _countDeallocated = 0;
    };

}
