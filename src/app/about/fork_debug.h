#pragma once
#include <QString>
class QWidget;

namespace kobalt::reader {
// Adds input handling only. Does not change the label, style, font or paint path.
void installForkLanguageDebug(QWidget *label, QWidget *host, const QString &directory);
} // namespace kobalt::reader
