#ifndef LH_AIDAWRITER_H
#define LH_AODAWRITER_H

#include "../LH_Text/LH_Text.h"
#include "../LH_Qt_QTextEdit.h"
#include <windows.h>





class LH_AidaWriter : public LH_Text
{
    Q_OBJECT

    HANDLE filemap_aida;
    char* aidaData;
    DWORD sz;
protected:
    LH_Qt_QTextEdit* setup_xml_;

public:
    LH_AidaWriter(const char *name);
    ~LH_AidaWriter();

    static lh_class *classInfo();

    int notify(int code,void* param);

    bool updateAidaMemory();
    bool createAidaMemory();
    void destroyAidaMemory();
};

#endif // LH_AIDAWRITER_H
