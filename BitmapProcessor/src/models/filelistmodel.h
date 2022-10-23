#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include <QtGui>
#include <QSortFilterProxyModel>
#include <QAbstractListModel>
#include <QQmlEngine>
#include <QFileInfo>

class FileListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    static FileListModel  *instance                                             (QObject *parent = nullptr);

    enum Roles           { DataRole = Qt::UserRole + 1,
                           FileIconRole,
                           FileNameRole,
                           FileSizeRole};

    Q_ENUM(Roles)

    // inteface
    QVariant               headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant               data                                        (const QModelIndex &index, int role) const override;
    int                    rowCount                             (const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames                                                                     () const override;

public:
    void                   addFileInfo                                              (const QFileInfo &info);

private:
    explicit               FileListModel                                        (QObject *parent = nullptr);
    QString                getFileSizeStr                                            (const quint64 &bytes) const;

private:
    static FileListModel  *m_instance;
    QList<QFileInfo>       m_fileList;
};
// -----------------------------------------------------------------------------
class FileListFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    static FileListFilterModel *instance                       (QObject *parent = nullptr);
    Q_INVOKABLE void            setSuffix                          (const QString &suffix);
    QVariant                    data                  (const QModelIndex &index, int role) const override;
    bool                        filterAcceptsRow       (int row, const QModelIndex &index) const override;

private:
    explicit                    FileListFilterModel            (QObject *parent = nullptr);
    static FileListFilterModel *m_instance;
    QString                     m_targetSuffix;
};
#endif // FILELISTMODEL_H
