#pragma once
class QWidget;
class QBoxLayout;

namespace kobalt::reader {
// Append only the fork build line in the shared About page. Stock rendering stays upstream.
void appendForkBuildRow(QWidget *page, QBoxLayout *rows, QWidget *host);
} // namespace kobalt::reader
