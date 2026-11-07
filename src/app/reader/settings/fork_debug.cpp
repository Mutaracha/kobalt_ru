#include "fork_debug.h"
#include "ui/dialogs/modals.h"
#include "ui/controls/options.h"
#include "ui/style/metrics.h"
#include "support/debug_language.h"
#include "support/strings.h"
#include <QApplication>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QPointer>
#include <QLabel>
#include <utility>

namespace kobalt::reader {
namespace {
class ForkLanguageDebug final : public QObject {
public:
    ForkLanguageDebug(QWidget *label, QWidget *host, QString directory)
        : QObject(label), label(label), host(host), directory(std::move(directory)) {
        label->installEventFilter(this);
        host->installEventFilter(this);
    }
protected:
    bool eventFilter(QObject *object, QEvent *event) override {
        if (object == host) {
            if (event->type() == QEvent::Resize && dialog) dialog->setGeometry(host->rect());
            return false;
        }
        const auto type = event->type();
        if (type == QEvent::Hide || type == QEvent::WindowDeactivate) {
            taps = 0;
            armed = false;
        }
        if (type == QEvent::MouseButtonPress || type == QEvent::MouseButtonDblClick) {
            const auto *mouse = static_cast<QMouseEvent *>(event);
            if (mouse->button() != Qt::LeftButton) return false;
            press = mouse->pos();
            armed = true;
            event->accept();
            return true;
        }
        if (type == QEvent::MouseButtonRelease) {
            const auto *mouse = static_cast<QMouseEvent *>(event);
            if (mouse->button() != Qt::LeftButton) return false;
            const bool tap = armed && label->rect().contains(mouse->pos()) &&
                (mouse->pos() - press).manhattanLength() <= QApplication::startDragDistance();
            armed = false;
            if (!tap) { taps = 0; return true; }
            if (!between.isValid() || between.elapsed() > 700) taps = 0;
            between.restart();
            if (++taps == 5) {
                taps = 0;
                showChoices();
            }
            event->accept();
            return true;
        }
        return false;
    }
private:
    void showChoices() {
        if (dialog || !host || !label->isVisible()) return;
        auto *popup = new ui::ModalOverlay(host);
        dialog = popup;
        popup->setObjectName("rrDebugLanguage");
        popup->setProperty("reader_standard_modal", true);
        popup->setDimmed(true);
        popup->setGeometry(host->rect());
        auto *card = popup->card();
        card->title(strings::get("debug.language_title"));
        auto *message = card->message(strings::get("debug.language_restart"));
        auto *choices = new ui::RadioList(card);
        // The modal action owns its top rule. Do not add a second rule under
        // the final radio row (or an unnecessary one above the first row).
        choices->setBottomDivider(false);
        choices->add("rrDebugLanguageEnglish", QStringLiteral("English"));
        choices->add("rrDebugLanguageRussian", QString::fromUtf8("Русский"));
        choices->add("rrDebugLanguageAutomatic", strings::get("debug.language_automatic"));
        card->rows()->addWidget(choices);
        auto *cancel = ui::makeModalButton(card, {strings::get("common.cancel"), "rrDebugLanguageClose"}, false);
        // ModalButton has no intrinsic minimum height: the standard adaptive
        // action layout must measure its text/padding at the current scale.
        card->adaptiveActions({cancel});
        const auto dismiss = [this, popup] {
            popup->hide();
            popup->deleteLater();
            dialog.clear();
        };
        connect(cancel, &QPushButton::clicked, this, dismiss);
        popup->dismissed = dismiss;
        choices->chosen = [this, choices, message, popup, cancel](int index) {
            choices->setEnabled(false);
            cancel->setEnabled(false);
            popup->dismissed = {}; // Do not submit another choice until this write completes.
            const auto language = index == 0 ? DebugLanguage::English :
                                  index == 1 ? DebugLanguage::Russian : DebugLanguage::Automatic;
            writeDebugLanguageAsync(directory, language, this, [this, choices, message, popup, cancel](bool saved) {
                if (!dialog || dialog != popup) return;
                message->setText(strings::get(saved ? "debug.language_saved" : "debug.language_failed"));
                choices->setVisible(!saved);
                choices->setEnabled(true);
                cancel->setText(strings::get("common.ok"));
                cancel->setEnabled(true);
                popup->dismissed = [this, popup] {
                    popup->hide(); popup->deleteLater(); dialog.clear();
                };
                popup->fitCard();
            });
        };
        popup->fitCard();
        popup->show();
        popup->raise();
        popup->setFocus();
    }
    QWidget *label;
    QPointer<QWidget> host;
    QString directory;
    QPointer<ui::ModalOverlay> dialog;
    QElapsedTimer between;
    QPoint press;
    int taps = 0;
    bool armed = false;
};
}
void installForkLanguageDebug(QWidget *label, QWidget *host, const QString &directory) {
    if (label && host && !directory.isEmpty()) new ForkLanguageDebug(label, host, directory);
}
} // namespace kobalt::reader
