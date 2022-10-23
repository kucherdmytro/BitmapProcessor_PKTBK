#include "filelistmodel.h"

FileListModel *FileListModel::m_instance = nullptr;
FileListModel *FileListModel::instance(QObject *parent)
{
    return !m_instance ? m_instance = new FileListModel(parent) : m_instance;
}
// -----------------------------------------------------------------------------
FileListModel::FileListModel(QObject *parent) : QAbstractListModel(parent)
{

}
// -----------------------------------------------------------------------------
QString FileListModel::getFileSizeStr(const quint64 &bytes) const
{
    if(bytes < 1024)
    {
        return QString::number(bytes) + "B";
    }
    else if(bytes >= 1024 && bytes < (1024 * 1024))
    {
        return QString::number(bytes / 1024) + "K";
    }
    else if(bytes >= (1024 * 1024) && bytes < (1024 * 1024 * 1024))
    {
        return QString::number(bytes / 1024 / 1024) + "M";
    }
    else
    {
        return QString::number(bytes / 1024 / 1024 / 1024) + "G";
    }
}
// -----------------------------------------------------------------------------
void FileListModel::addFileInfo(const QFileInfo &info)
{
    if(!m_fileList.contains(info))
    {
        m_fileList.append(info);
        beginInsertRows(QModelIndex(), m_fileList.indexOf(info), m_fileList.indexOf(info));
        insertRow(0);
        endInsertRows();

        emit dataChanged(createIndex(m_fileList.indexOf(info), 0), createIndex(m_fileList.indexOf(info), 0));
    }
}
// -----------------------------------------------------------------------------
int FileListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_fileList.size();
}
// -----------------------------------------------------------------------------
QHash<int, QByteArray> FileListModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[DataRole]     = "fileData";
    roles[FileIconRole] = "fileIcon";
    roles[FileNameRole] = "fileName";
    roles[FileSizeRole] = "fileSize";

    return roles;
}
// -----------------------------------------------------------------------------
QVariant FileListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index))
    {
        return false;
    }

    const QFileInfo &info = m_fileList.at(index.row());
    switch(role)
    {
    case DataRole:
        return QVariant::fromValue(m_fileList.at(index.row()));
        break;
    case FileIconRole:
        if(info.suffix() == "bmp")
        {
            return QVariant::fromValue(QString("qrc:/img/bmp.png"));
        }
        else if(info.suffix() == "png")
        {
            return QVariant::fromValue(QString("qrc:/img/png.png"));
        }
        else if(info.suffix() == "barch")
        {
            return QVariant::fromValue(QString("qrc:/img/barch.png"));
        }
        else
        {
            return QVariant();
        }
    case FileNameRole:
        return QVariant::fromValue(info.fileName());
        break;
    case FileSizeRole:
        return QVariant::fromValue(getFileSizeStr(info.size()));
        break;
    default:
        return QVariant();
        break;
    }
}
// -----------------------------------------------------------------------------
QVariant FileListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return (role != Qt::DisplayRole) ? QVariant() : (orientation == Qt::Horizontal) ? QString() : QString::number(section+1) + ")";
}
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
FileListFilterModel *FileListFilterModel::m_instance = nullptr;
FileListFilterModel *FileListFilterModel::instance(QObject *parent)
{
    return !m_instance ? m_instance = new FileListFilterModel(parent) : m_instance;
}
// -----------------------------------------------------------------------------
FileListFilterModel::FileListFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    m_targetSuffix = "";
    setSourceModel(FileListModel::instance());
    setFilterRole(FileListModel::DataRole);
}
// -----------------------------------------------------------------------------
void FileListFilterModel::setSuffix(const QString &suffix)
{
    m_targetSuffix = suffix;
    invalidateFilter();
}
// -----------------------------------------------------------------------------
QVariant FileListFilterModel::data(const QModelIndex &index, int role) const
{
    return QSortFilterProxyModel::data(index, role);
}
// -----------------------------------------------------------------------------
bool FileListFilterModel::filterAcceptsRow(int row, const QModelIndex &index) const
{
    const QFileInfo &info = qvariant_cast<QFileInfo>(sourceModel()->data(sourceModel()->index(row, 0, index), filterRole()));
    if(m_targetSuffix == "bmp")
    {
        return (info.suffix() == "bmp") ? true : false;
    }
    else if(m_targetSuffix == "barch")
    {
        return (info.suffix() == "barch") ? true : false;
    }
    else if(m_targetSuffix == "png")
    {
        return (info.suffix() == "png") ? true : false;
    }
    else
    {
        return true;
    }
}
