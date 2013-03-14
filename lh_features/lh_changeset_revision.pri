LH_FEATURES += lh_changeset_revision
defineTest(lh_changeset_revision) {
    hg_output=$$system(hg log -l 1 --template {node}/{rev} \"$$_PRO_FILE_PWD_\")
    LH_CHANGESET=$$section(hg_output,"/",0,0)
    isEmpty(LH_CHANGESET): return(false)
    LH_REVISION=$$section(hg_output,"/",1,1)
    isEmpty(LH_REVISION): LH_REVISION=1
    DEFINES+=$$quote(CHANGESET=\\\"$$LH_CHANGESET\\\")
    DEFINES+=$$quote(REVISION=$$LH_REVISION)
    export(DEFINES)
    return(true)
}
