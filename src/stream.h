#ifndef STREAM_H
#define STREAM_H
#include <QIODevice>

class MsgPackStream
{
public:
    MsgPackStream();
    MsgPackStream(QIODevice *d);
    MsgPackStream(QByteArray *a, QIODevice::OpenMode mode);
    MsgPackStream(const QByteArray &a);
    virtual ~MsgPackStream();

    void setDevice(QIODevice *d);
    QIODevice *device() const;
    bool atEnd() const;

    enum Status {Ok, ReadPastEnd, ReadCorruptData, WriteFailed};
    Status status() const;
    void resetStatus();
    void setStatus(Status status);

    MsgPackStream &operator>>(bool &b);
    MsgPackStream &operator>>(quint8 &u8);
    MsgPackStream &operator>>(quint16 &u16);
    MsgPackStream &operator>>(quint32 &u32);
    MsgPackStream &operator>>(quint64 &u64);
    MsgPackStream &operator>>(qint8 &i8);
    MsgPackStream &operator>>(qint16 &i16);
    MsgPackStream &operator>>(qint32 &i32);
    MsgPackStream &operator>>(qint64 &i64);
    MsgPackStream &operator>>(float &f);
    MsgPackStream &operator>>(double &d);
    MsgPackStream &operator>>(QString &str);
    MsgPackStream &operator>>(QByteArray &array);

    MsgPackStream &operator<<(bool b);
    MsgPackStream &operator<<(quint32 u32);
    MsgPackStream &operator<<(quint64 u64);
    MsgPackStream &operator<<(qint32 i32);
    MsgPackStream &operator<<(qint64 i64);
    MsgPackStream &operator<<(float f);
    MsgPackStream &operator<<(double d);
    MsgPackStream &operator<<(QString str);
    MsgPackStream &operator<<(const char *str);
    MsgPackStream &operator<<(QByteArray array);

private:
    QIODevice *dev;
    bool owndev;
    Status q_status;

    bool unpack_upto_quint8(quint8 &u8, quint8 *p);
    bool unpack_upto_quint16(quint16 &u16, quint8 *p);
    bool unpack_upto_quint32(quint32 &u32, quint8 *p);
    bool unpack_upto_quint64(quint64 &u64, quint8 *p);
    bool unpack_upto_qint8(qint8 &i8, quint8 *p);
    bool unpack_upto_qint16(qint16 &i16, quint8 *p);
    bool unpack_upto_qint32(qint32 &i32, quint8 *p);
    bool unpack_upto_qint64(qint64 &i64, quint8 *p);
};

template <typename T>
MsgPackStream& operator<<(MsgPackStream& s, const QList<T> &list)
{
    s << (quint32)list.size();
    for (int i = 0; i < list.size(); ++i)
        s << list[i];
    return s;
}

template <typename T>
MsgPackStream& operator>>(MsgPackStream& s, QList<T> &list)
{
    list.clear();
    quint32 size;
    s >> size;
    for (quint32 i = 0; i < size; ++i) {
        T t;
        s >> t;
        list.append(t);
        if (s.atEnd())
            break;
    }
    return s;
}

#endif // STREAM_H
