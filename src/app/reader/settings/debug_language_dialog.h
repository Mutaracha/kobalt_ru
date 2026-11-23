#pragma once
#include <QString>
class QWidget;
class QPushButton;

namespace kobalt::reader {
// Connects the Debug-page action to the existing persistent language chooser.
void installDebugLanguageChooser(QPushButton *button, QWidget *host, const QString &directory);
} // namespace kobalt::reader
