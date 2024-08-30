#include "HashUtils.h"
#include <QCryptographicHash>

QString sha256(const QString& input) {
    QByteArray byteArray = input.toUtf8(); // Convert QString to QByteArray
    QByteArray hashArray = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha256); // Perform SHA-256 hashing
    return QString(hashArray.toHex()); // Convert the hash to a hexadecimal string
}
