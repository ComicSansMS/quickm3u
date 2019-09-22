#ifndef INCLUDE_GUARD_QUICKM3U_UI_M3U_FILE_MODEL_HPP_
#define INCLUDE_GUARD_QUICKM3U_UI_M3U_FILE_MODEL_HPP_

#include <quickm3u/m3u_file.hpp>

#pragma warning(push)
#pragma warning(disable: 4251)
#include <QAbstractListModel>
#pragma warning(pop)

namespace ui {

class M3UFileModel : public QAbstractListModel {
    Q_OBJECT
private:
    M3UFile m_file;
public:
    M3UFileModel(QObject* parent);

    Qt::ItemFlags flags(QModelIndex const& index) const override;
    int rowCount(QModelIndex const& parent) const override;
    QVariant data(QModelIndex const& index, int role) const override;
    bool setData(QModelIndex const& index, QVariant const& value, int role) override;
    bool insertRows(int row, int count, QModelIndex const& parent) override;
    bool removeRows(int row, int count, QModelIndex const& parent) override;
    Qt::DropActions supportedDropActions() const override;
    bool dropMimeData(QMimeData const* data, Qt::DropAction action,
                      int row, int column, QModelIndex const& parent) override;

public slots:
    void openFile(QString const& path);
    void newFile();
};

}

#endif
