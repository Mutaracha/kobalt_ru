#include "fork_debug.h"
#include "ui/dialogs/modals.h"
#include "ui/controls/options.h"
#include "ui/style/metrics.h"
#include "support/debug_language.h"
#include "support/strings.h"
#include <QApplication>
#include <QPointer>
#include <QPushButton>
#include <utility>

namespace kobalt::reader {
namespace {
class ForkLanguageDebug final : public QObject
{
public:
    ForkLanguageDebug(QPushButton *button, QWidget *host, QString directory)
        : QObject(button), button(button), host(host), directory(std::move(directory))
    {
        connect(button, &QPushButton::clicked, this, [this] { showChoices(); });
        host->installEventFilter(this);
    }

protected:
    bool eventFilter(QObject *object, QEvent *event) override
    {
        if (object == host && event->type() == QEvent::Resize && dialog)
            dialog->setGeometry(host->rect());
        return false;
    }

private:
    void showChoices()
    {
        if (dialog || !host || !button->isVisible())
            return;
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
        // Show the persisted override, not the effective interface locale:
        // Automatic may also resolve to Russian on a localized device.
        const auto language = debugLanguage(directory);
        choices->setCurrent(language == DebugLanguage::English           ? 0
                                    : language == DebugLanguage::Russian ? 1
                                                                         : 2);
        card->rows()->addWidget(choices);
        auto *cancel = ui::makeModalButton(
                card, { strings::get("common.cancel"), "rrDebugLanguageClose" }, false);
        // ModalButton has no intrinsic minimum height: the standard adaptive
        // action layout must measure its text/padding at the current scale.
        card->adaptiveActions({ cancel });
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
            const auto language = index == 0 ? DebugLanguage::English
                    : index == 1             ? DebugLanguage::Russian
                                             : DebugLanguage::Automatic;
            writeDebugLanguageAsync(
                    directory, language, this, [this, choices, message, popup, cancel](bool saved) {
                        if (!dialog || dialog != popup)
                            return;
                        message->setText(strings::get(saved ? "debug.language_saved"
                                                            : "debug.language_failed"));
                        choices->setVisible(!saved);
                        choices->setEnabled(true);
                        cancel->setText(strings::get("common.ok"));
                        cancel->setEnabled(true);
                        popup->dismissed = [this, popup] {
                            popup->hide();
                            popup->deleteLater();
                            dialog.clear();
                        };
                        popup->fitCard();
                    });
        };
        popup->fitCard();
        popup->show();
        popup->raise();
        popup->setFocus();
    }

    QPushButton *button;
    QPointer<QWidget> host;
    QString directory;
    QPointer<ui::ModalOverlay> dialog;
};
} // namespace

void installForkLanguageDebug(QPushButton *button, QWidget *host, const QString &directory)
{
    if (button && host && !directory.isEmpty())
        new ForkLanguageDebug(button, host, directory);
}
} // namespace kobalt::reader
