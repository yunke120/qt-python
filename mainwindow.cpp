#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QException>
#include "Python.h"

enum Except{THROW_ZARO, THROW_ONE};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Py_SetPythonHome(L"./");
    Py_Initialize();

    QT_TRY
    {
        int re;
        re = PyRun_SimpleString("print('Qt: hello python')");
        //    re = PyRun_SimpleString("DDD");
        if(re != 0)
        {
            PyErr_Print();
            return;
        }

        const char *filename = "./mymodule.py";
        FILE *fp = fopen(filename, "r");
        if(!fp)
        {
            QT_THROW (THROW_ZARO);
        }
        re = PyRun_AnyFile(fp, filename);
        if(re != 0)
        {
            PyErr_Print();
            QT_THROW (THROW_ZARO);
        }

        /* 获取主模块 */
        PyObject *m = NULL;
        {
            PyObject *key = PyUnicode_FromString("__main__");
            m = PyImport_GetModule(key);
            Py_XDECREF(key);
            PyErr_Print();
        }
        /* 读取字典 */
        {
            PyObject *conf = PyObject_GetAttrString(m, "conf");
            if(!conf)
            {
                QT_THROW (THROW_ZARO);
            }
            PyObject *key = PyUnicode_FromString("width");
            int w = PyLong_AsLong(PyDict_GetItem(conf, key));
            Py_XDECREF(key);

            key = PyUnicode_FromString("height");
            int h = PyLong_AsLong(PyDict_GetItem(conf, key));
            Py_XDECREF(key);

            key = PyUnicode_FromString("title");
            wchar_t title[1024] = {0};
            int size = PyUnicode_AsWideChar(PyDict_GetItem(conf, key), title, 1023);
            Py_XDECREF(key);

            /* python 类 */
            {
                PyObject *class_ = PyObject_GetAttrString(m, "MyPython");   /* 获取类 */
                if(!class_)
                {
                    QT_THROW (THROW_ZARO);
                    return;
                }

                PyObject *obj = PyObject_CallObject(class_, NULL);          /* 实例化类 */
                if(!obj)
                {
                    QT_THROW (THROW_ZARO);
                    return;
                }

                PyObject_CallMethod(obj, "nRnArg", NULL);     /* 调用无参无返回方法 */
                PyObject_CallMethod(obj, "nRyArg", "ii", 1,2);     /* 调用有参无返回方法 */
                PyObject *re =  PyObject_CallMethod(obj, "yRyArg", "ii", 1,2); /* 调用有参有返回方法 */
                qDebug() << "Qt: yRyArg return:" << PyLong_AsLong(re);
                Py_XDECREF(re);

                re =  PyObject_CallMethod(obj, "yRnArg", NULL);
                if (!re || !PyUnicode_Check(re)) {
                    qDebug() << __LINE__;
                    return;
                }
                PyObject* utf8StrObj = PyUnicode_AsUTF8String(re);
                if (!utf8StrObj) {
                    return;
                }
                const char* utf8Str = PyBytes_AsString(utf8StrObj);
                if (!utf8Str) {
                    Py_DECREF(utf8StrObj);
                    return;
                }
                qDebug() << "Qt: " << QString::fromUtf8(utf8Str);
                Py_XDECREF(utf8StrObj);
                Py_XDECREF(re);


                re = PyObject_GetAttrString(obj, "id");
                if(!re)
                {
                    QT_THROW (THROW_ZARO);
                }
                qDebug()<< "Qt: " << PyLong_AsLong(re);
                Py_XDECREF(re);


                Py_XDECREF(obj);
                Py_XDECREF(class_);

            }

            /* 调用函数 */
            {
                PyObject *Main = PyObject_GetAttrString(m, "Main"); // 无参无返回
                if(Main && PyCallable_Check(Main))
                {
                    if(!PyObject_CallObject(Main, NULL))
                    {
                        PyErr_Print();
                        QT_THROW (THROW_ZARO);
                    }
                }
                Py_XDECREF(Main);

                PyObject *RList = PyObject_GetAttrString(m, "RList"); // 无参无返回
                if(RList && PyCallable_Check(RList))
                {
                    PyObject *args = PyTuple_New(1);
                    PyObject *list = PyList_New(0);
                    for(int i = 0; i< 5; i++)
                        PyList_Append(list, PyLong_FromLong(i+100));
                    PyTuple_SetItem(args, 0, list);

                    PyObject *re = PyObject_CallObject(RList, args);
                    if(re)
                    {
                        int size = PyList_Size(re);
                        for(int i = 0; i < size; i++)
                        {
                            PyObject *val = PyList_GetItem(re, i);
                            if(!val) continue;
                            qDebug() << "Qt: " << PyLong_AsLong(val);
                        }
                    }


                    Py_XDECREF(re);
                    Py_XDECREF(args);
                    Py_XDECREF(RList);
                }

            }

            wprintf(L"title = %s", title);
            qDebug() << w << h << size;
            Py_XDECREF(conf);
        }

        /* 传递变量 */
        {
            PyRun_SimpleString("a = 999");
            PyObject_SetAttrString(m, "count", PyLong_FromLong(555)); /* 空间转给python管理 */
        }
        Py_XDECREF(m);
        Py_Finalize();
    }
    QT_CATCH (Except &ex)
    {
        if(ex == THROW_ZARO) qDebug() << "open python moudle failed";
        Py_Finalize();
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}
