/**
  \file     LH_QtInstance.h
  \author   Johan Lindh <johan@linkdata.se>
  \legalese Copyright (c) 2009-2010 Johan Lindh

  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of Link Data Stockholm nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
  */

#ifndef LH_QTINSTANCE_H
#define LH_QTINSTANCE_H

#include <QtGlobal>
#include <QImage>

#include "LH_QtObject.h"
#include "LH_QtLayoutClass.h"

class LH_QtPlugin;

/**
  Base class for LCDHost layout classes and layout items using Qt.
  For normal use, the macro LH_PLUGIN_CLASS(classname) will export
  the class from the implementation file (not from the header file!).

  See LH_Lua for an example on how to handle dynamically creating and
  removing classes.
  */
class LH_QtInstance : public LH_QtObject
{
    Q_OBJECT

protected:
    lh_layout_item li_;
    QImage *image_;

public:
    LH_QtInstance( LH_QtLayoutClass *parent = 0 );
    ~LH_QtInstance();

    // supporting argumentless constructors and auto-registrating layout classes
    lh_layout_item& layoutItem(LH_QtLayoutClass *qlc) { setParent(qlc); return li_; }

    LH_QtLayoutClass *parent() const { return static_cast<LH_QtLayoutClass *>(LH_QtObject::parent()); }
    bool isMonochrome() const { return li_.layout.depth == 1; }

    lh_layout_item *item() { return &li_; }
    QImage *image() const { return image_; }
    QImage *initImage(int w, int h);

    virtual void prerender() {}
    virtual int width( int ) { return -1; }
    virtual int height( int ) { return -1; }
    virtual lh_blob *render_blob( int, int ) { return NULL; }
    virtual QImage *render_qimage( int, int ) { return NULL; }

    virtual QString dir_layout() const
    {
        Q_ASSERT( li_.layout.dir );
        return QString::fromUtf8( li_.layout.dir );
    }

    /* reimplement this static member in your own classes */
    static const lh_class *classInfo()
    {
        Q_ASSERT(!"You must reimplement classInfo()");
        return 0;
    }

public slots:
    void show() const { int b = 0; callback( lh_cb_sethidden, (void*)&b ); }
    void hide() const { int b = 1; callback( lh_cb_sethidden, (void*)&b ); }
    void setVisible( bool b ) const { int notb = !b; callback( lh_cb_sethidden, (void*)&notb ); }
};

/**
  This macro creates the required things to allow
  automatic registration of layout classes. Note that using this
  macro requires a static classInfo() method that returns a
  statically allocated lh_class structure pointer.
  */
#define LH_PLUGIN_CLASS(classname) \
    class lh_layout_class_##classname : public LH_QtLayoutClass { \
    public: lh_layout_class_##classname( const lh_class *cls, LH_QtObject *parent ) : \
    LH_QtLayoutClass(cls,parent) {} \
    lh_layout_item *layout_item_create() { return &(new classname())->layoutItem(this); } \
    void layout_item_destroy(lh_layout_item *li) { delete reinterpret_cast<classname*>(li->obj.ref); } \
    }; \
    static void lh_load_##classname(LH_QtObject *parent) { \
        new lh_layout_class_##classname( classname::classInfo(), parent ); } \
    LH_QtLoader lh_qtloader_##classname(lh_load_##classname);

#endif // LH_QTINSTANCE_H
