#pragma once
#include "engine/crengine/column_view.h"
#include "engine/crengine/engine_document.h"
#include <algorithm>

namespace kobalt {
// Independent copies of the pre-index algorithms for correctness and timing.
// Test-only engine access; no application API for pagination internals.
struct ReaderSourceIndexProbe {
    using Ranges = QVector<QPair<QString, QString>>;
    static bool comicsGeometry(const EngineDocument &model) {
        const auto margins = model.engine_->getPageMargins();
        return margins.left == 0 && margins.top == 0 && margins.right == 0
                && margins.bottom == 0 && model.columns_ == 1;
    }
    static int inPageFootnoteCount(const EngineDocument &model) {
        if (!model.engine_)
            return 0;
        const int page = model.engine_->getCurPage(true);
        const auto *pages = model.engine_->getPageList();
        return page >= 0 && page < pages->length() ? (*pages)[page]->footnotes.length() : 0;
    }
    static int footnotePageCount(const EngineDocument &model) {
        const auto *pages = model.engine_->getPageList();
        int result = 0;
        for (int i = 0; i < pages->length(); ++i)
            result += (*pages)[i]->footnotes.length() > 0;
        return result;
    }
    static QRect nodeBounds(const EngineDocument &model, const char *id) {
        auto *node = model.engine_->getDocument()->getElementById(cre(QString::fromLatin1(id)).c_str());
        if (!node) return {};
        lvRect rect;
        node->getAbsRect(rect);
        return QRect(rect.left, rect.top, rect.width(), rect.height());
    }
    static int nodeFontPixels(const EngineDocument &model, const char *id) {
        auto *node = model.engine_->getDocument()->getElementById(cre(QString::fromLatin1(id)).c_str());
        return node ? node->getFont()->getSize() : 0;
    }
    static void useReferenceStyleSheet(EngineDocument &model, const QByteArray &css) {
        model.engine_->setStyleSheet(lString8(css.constData()));
        model.engine_->requestRender();
        model.engine_->Render();
    }
    static int fontPixelsAt(const EngineDocument &model, const QString &position) {
        const auto pointer = model.engine_->getDocument()->createXPointer(cre(position));
        auto *node = pointer.isNull() ? nullptr : pointer.getNode();
        if (node && !node->isElement())
            node = node->getParentNode();
        return node ? node->getFont()->getSize() : 0;
    }
    static bool inPageHint(const EngineDocument &model, const char *id) {
        auto *node = model.engine_->getDocument()->getElementById(cre(QString::fromLatin1(id)).c_str());
        return node && !node->getStyle().isNull() && STYLE_HAS_CR_HINT(node->getStyle(), FOOTNOTE_INPAGE);
    }
    static ldomNode *noteHeading(const EngineDocument &model, const char *id) {
        auto *node = model.engine_->getDocument()->getElementById(cre(QString::fromLatin1(id)).c_str());
        QVector<ldomNode *> pending;
        if (node)
            pending.append(node);
        // CREngine may wrap inline/title blocks while rendering. Inspect the
        // owned subtree, not just the pre-render direct-child shape.
        while (!pending.isEmpty()) {
            auto *child = pending.takeLast();
            if (child->isNodeName("title"))
                return child;
            for (int i = child->getChildCount() - 1; i >= 0; --i)
                pending.append(child->getChildNode(i));
        }
        return nullptr;
    }
    static QString noteTitle(const EngineDocument &model, const char *id) {
        const auto *title = noteHeading(model, id);
        return title ? QString::fromUtf8(UnicodeToUtf8(title->getText()).c_str()).trimmed() : QString();
    }
    static bool numericNoteTitle(const EngineDocument &model, const char *id) {
        const auto *title = noteHeading(model, id);
        return title && title->getAttributeValue("data-kobalt-note-number") == U"1";
    }
    static QString nodeAttribute(const EngineDocument &model, const char *id, const char *name) {
        auto *node = model.engine_->getDocument()->getElementById(cre(QString::fromLatin1(id)).c_str());
        return node ? QString::fromUtf8(UnicodeToUtf8(node->getAttributeValue(name)).c_str()) : QString();
    }
    static QString normalizationDiagnostic(const EngineDocument &model, const char *id) {
        auto *node = model.engine_->getDocument()->getElementById(cre(QString::fromLatin1(id)).c_str());
        QStringList result;
        for (; node; node = node->getParentNode()) {
            const auto str = [](const lString32 &value) {
                return QString::fromUtf8(UnicodeToUtf8(value).c_str());
            };
            result.append(QString("%1[paragraph=%2,beautify=%3,text=%4]")
                          .arg(str(node->getNodeName()), str(node->getAttributeValue("data-kobalt-paragraph")),
                               str(node->getAttributeValue("data-kobalt-beautify")))
                          .arg(str(node->getText()).size()));
            const auto style = node->getStyle();
            if (!style.isNull())
                result.append(QString("align=%1 white=%2 indent=%3 margins=%4,%5,%6,%7 padding=%8,%9,%10,%11")
                              .arg(int(style->text_align)).arg(int(style->white_space)).arg(style->text_indent.value)
                              .arg(style->margin[0].value).arg(style->margin[1].value)
                              .arg(style->margin[2].value).arg(style->margin[3].value)
                              .arg(style->padding[0].value).arg(style->padding[1].value)
                              .arg(style->padding[2].value).arg(style->padding[3].value));
        }
        return result.join(" | ");
    }
    static lString32 cre(const QString &text) { return Utf8ToUnicode(text.toUtf8().constData()); }

    static void clear(EngineDocument &model) { model.sourceIndex_.reset(); }

    static QPair<int, qint64> usage(const EngineDocument &model)
    {
        return model.sourceIndexUsage();
    }

    static bool cached(const EngineDocument &model) { return bool(model.sourceIndex_); }

    static bool positionLookupPreservesNames(const EngineDocument &model, bool normalized)
    {
        auto *doc = model.engine_->getDocument();
        const char *name = normalized ? "unknown_position_v2" : "unknown_position_v1";
        if (doc->findElementNameIndex(name))
            return false;
        const auto path = cre('/' + QString::fromLatin1(name) + "[1]");
        const auto version = doc->getDOMVersionRequested();
        doc->setDOMVersionRequested(normalized ? DOM_VERSION_WITH_NORMALIZED_XPOINTERS
                                               : DOM_VERSION_WITH_NORMALIZED_XPOINTERS - 1);
        const auto pointer = doc->createXPointer(doc->getRootNode(), path);
        doc->setDOMVersionRequested(version);
        return pointer.isNull() && !doc->findElementNameIndex(name);
    }

    static QVector<int> visible(const EngineDocument &model, const Ranges &ranges)
    {
        QVector<int> result;
        if (ranges.isEmpty() || ranges.size() > 2001 || !model.engine_ || model.pages() <= 0)
            return result;
        auto *doc = model.engine_->getDocument();
        const int current = model.page();
        const ldomXPointerEx first(model.engine_->getPageBookmark(current));
        const ldomXPointerEx last = current + 1 < model.pages()
                ? ldomXPointerEx(model.engine_->getPageBookmark(current + 1))
                : ldomXRange(doc->getRootNode(), true).getEnd();
        if (first.isNull() || last.isNull())
            return result;
        for (int i = 0; i < ranges.size(); ++i) {
            const auto &range = ranges[i];
            if (range.first.isEmpty() || range.second.isEmpty() || range.first.size() > 4096
                || range.second.size() > 4096)
                continue;
            ldomXPointerEx start(doc->createXPointer(cre(range.first))),
                    end(doc->createXPointer(cre(range.second)));
            if (start.isNull() || end.isNull())
                continue;
            if (start.compare(end) > 0)
                std::swap(start, end);
            if (start.compare(end) < 0 && start.compare(last) < 0 && first.compare(end) < 0)
                result.append(i);
        }
        return result;
    }

    static QVector<int> overlaps(const EngineDocument &model, const QString &start,
                                 const QString &end, const Ranges &ranges)
    {
        QVector<int> result;
        if (!model.engine_)
            return result;
        auto resolve = [&model](const QString &anchor) {
            if (anchor.isEmpty() || anchor.size() > 4096)
                return ldomXPointerEx();
            ldomXPointerEx pointer(model.engine_->getDocument()->createXPointer(cre(anchor)));
            if (pointer.isNull() || !pointer.isText() || pointer.getOffset() < 0
                || pointer.getOffset() > pointer.getNode()->getText().length()
                || !pointer.isVisible())
                return ldomXPointerEx();
            return pointer;
        };
        const auto first = resolve(start), last = resolve(end);
        if (first.isNull() || last.isNull() || first.compare(last) >= 0)
            return result;
        for (int i = 0; i < ranges.size(); ++i) {
            const auto a = resolve(ranges[i].first), b = resolve(ranges[i].second);
            if (!a.isNull() && !b.isNull() && a.compare(b) < 0 && a.compare(last) < 0
                && first.compare(b) < 0)
                result.append(i);
        }
        return result;
    }

    static QString shifted(const EngineDocument &model, const QString &anchor, int amount)
    {
        auto point = model.engine_->getDocument()->createXPointer(cre(anchor));
        if (point.isNull())
            return {};
        point.setOffset(point.getOffset() + amount);
        return QString::fromUtf8(UnicodeToUtf8(point.toString()).c_str());
    }

    static QString textPoint(const EngineDocument &model, const QString &needle, int offset)
    {
        std::function<ldomNode *(ldomNode *)> find = [&](ldomNode *node) -> ldomNode * {
            if (node->isText()
                && QString::fromUtf8(UnicodeToUtf8(node->getText()).c_str()).contains(needle))
                return node;
            for (int i = 0; i < node->getChildCount(); ++i)
                if (auto *found = find(node->getChildNode(i)))
                    return found;
            return nullptr;
        };
        auto *node = find(model.engine_->getDocument()->getRootNode());
        return node
                ? QString::fromUtf8(UnicodeToUtf8(ldomXPointer(node, offset).toString()).c_str())
                : QString();
    }

    static QVector<int> pages(const EngineDocument &model, const QStringList &anchors,
                              bool endpoints, QVector<int> *order = nullptr)
    {
        QVector<int> result;
        QVector<ldomXPointerEx> pointers;
        if (order) {
            pointers.resize(anchors.size());
            order->fill(-1, anchors.size());
        }
        if (!model.engine_) {
            result.fill(-1, anchors.size());
            return result;
        }
        for (int i = 0; i < anchors.size(); ++i) {
            ldomXPointerEx pointer(
                    anchors[i].isEmpty()
                            ? ldomXPointer()
                            : model.engine_->getDocument()->createXPointer(cre(anchors[i])));
            if (order)
                pointers[i] = pointer;
            if (endpoints && i % 2 == 1 && !pointer.isNull())
                pointer.prevVisibleChar(false);
            result.append(pointer.isNull() ? -1 : model.engine_->getBookmarkPage(pointer));
        }
        if (order) {
            QVector<int> indices;
            for (int i = 0; i < pointers.size(); ++i)
                if (!pointers[i].isNull())
                    indices.append(i);
            std::stable_sort(indices.begin(), indices.end(),
                             [&](int a, int b) { return pointers[a].compare(pointers[b]) < 0; });
            for (int i = 0; i < indices.size(); ++i)
                (*order)[indices[i]] = i;
        }
        return result;
    }

    static QString childOffset(const EngineDocument &model, const QString &anchor)
    {
        auto point = model.engine_->getDocument()->createXPointer(cre(anchor));
        if (point.isNull())
            return {};
        auto *parent = point.getNode()->getParentNode();
        if (!parent || !parent->getChildCount())
            return {};
        ldomXPointer child(parent, qMin(1, parent->getChildCount()));
        return QString::fromUtf8(UnicodeToUtf8(child.toString()).c_str());
    }
};
} // namespace kobalt
