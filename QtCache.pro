CONFIG += ordered qtcachetool
TEMPLATE = subdirs

SUBDIRS = \
    QtCache \
    QtCacheUi

qtcachetool{
    SUBDIRS *=  \
        QtCacheTool
}

samples{
    SUBDIRS *=  \
        QtCachePluginSample
}

gitplugin{
    SUBDIRS *=  \
        QtCacheGitPlugin
}
