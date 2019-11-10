#ifndef INCLUDE_GUARD_QUICKM3U_UI_M3U_FILE_MODEL_HPP_
#define INCLUDE_GUARD_QUICKM3U_UI_M3U_FILE_MODEL_HPP_

#include <quickm3u/m3u_file.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
#include <QAbstractListModel>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <deque>

namespace ui {

class M3UFileModel : public QAbstractListModel {
    Q_OBJECT
private:
    std::deque<M3UFile> m_undoHistory;
    std::deque<M3UFile> m_redoHistory;
    int m_lastInsertIndex;
    M3UFile m_file;
    int m_limitUndoHistory;
public:
    M3UFileModel(QObject* parent);

    Qt::ItemFlags flags(QModelIndex const& index) const override;
    int rowCount(QModelIndex const& parent) const override;
    QVariant data(QModelIndex const& index, int role) const override;
    bool setData(QModelIndex const& index, QVariant const& value, int role) override;
    bool insertRows(int row, int count, QModelIndex const& parent) override;
    bool removeRows(int row, int count, QModelIndex const& parent) override;
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QMimeData* mimeData(QModelIndexList const& indices) const override;
    bool dropMimeData(QMimeData const* data, Qt::DropAction action,
                      int row, int column, QModelIndex const& parent) override;
    bool gatherRows(int* source_rows_ptr, std::size_t source_rows_size, int destination_row);

    QString getFilename() const;
    QString getFullPath() const;
signals:
    void pathChanged();
    void historyChange(bool can_undo, bool can_redo);
public slots:
    void newFile(QString const& path);
    void openFile(QString const& path);
    void saveFile();
    void convertToRelativePaths();
    void convertToAbsolutePaths();
    void undo();
    void redo();
    void copyFilesToDirectory(QString destination_path);
private:
    void saveState();
};

}

#endif
