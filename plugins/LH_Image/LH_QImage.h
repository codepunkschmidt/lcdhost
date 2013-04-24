#ifndef LH_QIMAGE_H
#define LH_QIMAGE_H

#include "LH_Image.h"
#include "LH_QtCFInstance.h"

class LH_IMAGE_EXPORT LH_QImage : public LH_QtCFInstance
{
    Q_OBJECT

protected:
    LH_Qt_QFileInfo *setup_image_file_;
    LH_Qt_bool *setup_show_placeholder_;

    virtual bool loadPlaceholderImage(QImage *img);

public:
    LH_QImage() : LH_QtCFInstance(), setup_image_file_(0), setup_show_placeholder_(0) {}
    const char *userInit();
    QImage *render_qimage( int w, int h );

    //static lh_class *classInfo();
};

#endif // LH_QIMAGE_H
