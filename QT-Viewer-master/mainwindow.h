#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "interactiveview.h"
#include <QMainWindow>
#include <QString>
#include <QDir>
#include <QFileInfoList>
#include <QLabel>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QPixmap>
#include <QRect>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    void initStatusBar();
    void updateinfo();

    void showImg();
    void showPicture(QString filePath);

    int getIndex(QString string);
    bool CheckIsSelect(int index);
    void updateSelectStatus();
    int get_prev_index(int index);
    int get_next_index(int index);

protected:
    void resizeEvent(QResizeEvent * event);

    //槽
private slots:
    //关闭窗口
    void on_closeWin_action_triggered();
    //打开图片
    void on_open_action_triggered();
    //文件改变
    void fileChanged(const QModelIndex &index);
    //关闭图片
    void on_closePic_action_triggered();
    //放大
    void on_zoomin_triggered();
    //缩小
    void on_zoomout_triggered();
    //上一张
    void on_up_triggered();
    //下一张
    void on_down_triggered();
private slots:
    void on_select_triggered();

    //还原
    void on_restore_triggered();

    void on_unselect_triggered();

private:
    Ui::MainWindow *ui;
    InteractiveView *interView;


    int curindex; //用来判断第几张


    QString curPath; //当前路径
    QString curFileName; //当前文件名
    QDir curDir; //当前文件夹
    QFileInfoList fileInfoList;  //图像列表
    QFileSystemModel *model; //给listview使用的过滤器
    QPixmap pix;   //图像


    double level;//放大系数

    QRect screenRect; //屏幕尺寸

    QLabel* fileIndexLabel;
    QLabel* filePathLabel;
    QLabel* fileSizeLabel;
    QLabel* imageScaleLabel;
    QLabel* imageSizeLabel;
    QLabel* fileModDateLabel;

    QList<int> select_index_list;

};

#endif // MAINWINDOW_H
