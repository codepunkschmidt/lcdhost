/**
  \file     LH_QtInstance.cpp
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

#include <QDebug>
#include "LH_QtInstance.h"

/**

  */
LH_QtInstance::LH_QtInstance( const char *name, const lh_class *cls, LH_QtPlugin *parent ) : LH_QtObject(name,parent)
{
    Q_UNUSED(cls);
    image_ = NULL;

    /**
      Sample code to add a setup item:

      \code
        QColor pencolor_; // in header
        lh_setup_item setup_pencolor_; // in header

        pencolor_ = QColor( Qt::black );
        setup_pencolor_.name = "Pen color";
        setup_pencolor_.type = lh_type_integer_color;
        setup_pencolor_.data.i = pencolor_.rgba();
        setup_pencolor_.flags = 0;
        setup_item_vector.append( &setup_pencolor_ );
      \endcode
      */

    return;
}

LH_QtInstance::~LH_QtInstance()
{
    if( image_ )
    {
        delete image_;
        image_ = NULL;
    }
    return;
}

/**
  Basic QImage handling. Call this at the start of
  your render_qimage to ensure that \c image_ is
  of the correct size. Always check the return
  value, and return NULL if the return value is
  NULL.
  */
QImage *LH_QtInstance::initImage(int w, int h)
{
    if( image_ && ( w != image_->width() || h != image_->height() ) )
    {
        delete image_;
        image_ = NULL;
    }
    if( image_ == NULL )
    {
        image_ = new QImage(w,h,QImage::Format_ARGB32_Premultiplied);
        if( image_ && image_->isNull() )
        {
            delete image_;
            image_ = NULL;
        }
    }
    return image_;
}
