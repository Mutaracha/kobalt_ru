# Полный список отличающихся runtime/build/assets/tests файлов
Группировка — навигация, не разрешение копировать файл целиком. Один файл может обслуживать несколько доработок. Назначение и проверки — в [карте](README.md).

## android-integration
| Файл | Статус |
|---|---|
| `platforms/android/CMakeLists.txt` | different |
| `platforms/android/app/src/org/kobalt/android/KobaltActivity.java` | different |
| `platforms/android/main.cpp` | different |

## assets-ru-fonts-icons-notices
| Файл | Статус |
|---|---|
| `assets/CMakeLists.txt` | different |
| `assets/acknowledgments.json` | different |
| `assets/en.strings` | different |
| `assets/hyph/Russian_EnUS.pattern` | fork-only |
| `assets/icons/phosphor/dark-theme.svg` | fork-only |
| `assets/manifest.json` | different |
| `assets/notices/Fluent-MIT` | fork-only |
| `assets/notices/Hyphenation-patterns` | different |
| `assets/notices/NickelFix-MIT` | fork-only |
| `assets/ru.strings` | fork-only |

## build-tools-platform
| Файл | Статус |
|---|---|
| `.clang-format-ignore` | different |
| `.github/workflows/ci.yml` | different |
| `.github/workflows/kobo-qt5-device.yml` | fork-only |
| `platforms/checks/reader-properties.allow` | different |
| `platforms/ci/check-source-archive.sh` | different |
| `platforms/ci/cleanup-runs.sh` | fork-only |
| `platforms/ci/test_upstream_delta.py` | fork-only |
| `platforms/ci/upstream_delta.py` | fork-only |
| `platforms/desktop/CMakeLists.txt` | different |
| `platforms/desktop/main.cpp` | different |
| `platforms/shared-inputs.cmake` | different |
| `platforms/strings-test.cmake` | different |
| `platforms/strings.cmake` | different |

## fb2-comics-engine-integration
| Файл | Статус |
|---|---|
| `src/engine/blaze/adapter/column_view.h` | different |
| `src/engine/blaze/adapter/column_view_comics.cpp` | fork-only |
| `src/engine/blaze/adapter/engine_document.cpp` | different |
| `src/engine/blaze/adapter/engine_document.h` | different |
| `src/engine/blaze/adapter/engine_document_comics.cpp` | fork-only |
| `src/engine/blaze/adapter/page_draw_buffer.cpp` | fork-only |
| `src/engine/blaze/adapter/page_draw_buffer.h` | fork-only |
| `src/engine/blaze/adapter/reader_alignment.cpp` | different |
| `src/engine/blaze/adapter/reader_beautifier.cpp` | different |
| `src/engine/blaze/adapter/reader_comics.cpp` | fork-only |
| `src/engine/blaze/adapter/reader_comics.h` | fork-only |
| `src/engine/blaze/adapter/reader_fb2.cpp` | fork-only |
| `src/engine/blaze/adapter/reader_fb2.h` | fork-only |
| `src/engine/blaze/adapter/reader_fork_styles.cpp` | fork-only |
| `src/engine/blaze/adapter/reader_fork_styles.h` | fork-only |
| `src/engine/blaze/adapter/reader_links.cpp` | different |
| `src/engine/blaze/adapter/reader_page_layout.cpp` | different |
| `src/engine/blaze/adapter/reader_source.cpp` | different |
| `src/engine/blaze/adapter/reader_source.h` | different |
| `src/engine/blaze/adapter/reader_styles.h.in` | different |
| `src/engine/engine.h` | different |
| `src/publication/fb2.h` | fork-only |
| `src/publication/fb2_resources.cpp` | fork-only |
| `src/publication/fb2_resources.h` | fork-only |
| `src/publication/image_request_trace.h` | fork-only |
| `src/publication/local_resources.cpp` | different |
| `src/publication/publication.cpp` | different |
| `src/publication/publication.h` | different |
| `src/publication/publication_fb2.cpp` | fork-only |
| `src/publication/resource_cache.cpp` | different |
| `src/publication/resource_cache.h` | different |

## kobo-integration
| Файл | Статус |
|---|---|
| `platforms/kobo/CMakeLists.txt` | different |
| `platforms/kobo/build.sh` | different |
| `platforms/kobo/common/book_menu.cpp` | different |
| `platforms/kobo/common/book_menu.h` | different |
| `platforms/kobo/common/page_button_mapping.h` | fork-only |
| `platforms/kobo/common/reader_coordinator.cpp` | different |
| `platforms/kobo/nickel-qt5/native_publication.cpp` | different |
| `platforms/kobo/nickel-qt5/native_ui.cpp` | different |
| `platforms/kobo/nickel-qt5/reader_controller.cpp` | different |
| `platforms/kobo/nickel-qt5/refresh.cpp` | different |
| `platforms/kobo/nickel-qt6/native_publication.cpp` | different |
| `platforms/kobo/nickel-qt6/reader_controller.cpp` | different |
| `platforms/kobo/sources.cmake` | different |

## locale-about-language
| Файл | Статус |
|---|---|
| `src/app/about/about_page.cpp` | different |
| `src/app/about/fork_about.cpp` | fork-only |
| `src/app/about/fork_about.h` | fork-only |
| `src/support/build_info.h.in` | different |
| `src/support/debug_language.cpp` | fork-only |
| `src/support/debug_language.h` | fork-only |
| `src/support/locale.cpp` | fork-only |
| `src/support/locale.h` | fork-only |
| `src/support/string_locale.h` | fork-only |
| `src/support/string_table.h` | fork-only |
| `src/support/strings.h` | different |

## reader-hooks-settings-navigation
| Файл | Статус |
|---|---|
| `src/app/library/library_view.cpp` | different |
| `src/app/reader/panels/activity_view.cpp` | different |
| `src/app/reader/panels/chapter_contents.cpp` | different |
| `src/app/reader/panels/chapter_graph.cpp` | different |
| `src/app/reader/panels/light_panel.cpp` | different |
| `src/app/reader/panels/search_panel.cpp` | different |
| `src/app/reader/settings/appearance_panel.cpp` | different |
| `src/app/reader/settings/appearance_panel.h` | different |
| `src/app/reader/settings/debug_language_dialog.cpp` | fork-only |
| `src/app/reader/settings/debug_language_dialog.h` | fork-only |
| `src/app/reader/settings/fork_settings.cpp` | fork-only |
| `src/app/reader/settings/fork_settings.h` | fork-only |
| `src/app/reader/settings/settings_panel.cpp` | different |
| `src/app/reader/views/fork_navigation.cpp` | fork-only |
| `src/app/reader/views/fork_navigation.h` | fork-only |
| `src/app/reader/views/navigation.cpp` | different |
| `src/app/reader/views/navigation.h` | different |
| `src/app/reader/views/reading_page.cpp` | different |
| `src/app/reader/views/reading_page.h` | different |
| `src/app/reader/window/fork_reader_actions.cpp` | fork-only |
| `src/app/reader/window/reader.cpp` | different |
| `src/app/reader/window/reader_annotations.cpp` | different |
| `src/app/reader/window/reader_build.cpp` | different |
| `src/app/reader/window/reader_p.h` | different |
| `src/app/reader/window/reader_refresh.cpp` | different |
| `src/app/reader/window/reader_settings.cpp` | different |
| `src/app/reader/window/reader_text_presets.cpp` | different |
| `src/reading/appearance_settings.cpp` | different |
| `src/reading/appearance_settings.h` | different |
| `src/reading/features.h` | different |
| `src/reading/fork_text_preset_names.h` | fork-only |
| `src/reading/opening_job.cpp` | different |
| `src/reading/page_navigation.h` | different |
| `src/reading/reader_session.cpp` | different |
| `src/reading/reader_session.h` | different |

## shared-ui-presentation
| Файл | Статус |
|---|---|
| `src/ui/controls/buttons.cpp` | different |
| `src/ui/controls/buttons.h` | different |
| `src/ui/controls/fork_icon_geometry.cpp` | fork-only |
| `src/ui/controls/fork_icon_geometry.h` | fork-only |
| `src/ui/controls/highlight_palette.h` | different |
| `src/ui/controls/icons.cpp` | different |
| `src/ui/controls/icons.h` | different |
| `src/ui/controls/options.cpp` | different |
| `src/ui/controls/options.h` | different |
| `src/ui/controls/return_title_text.h` | fork-only |

## sleep-presentation
| Файл | Статус |
|---|---|
| `src/app/reader/views/sleep_designs.cpp` | different |
| `src/app/reader/views/sleep_ribbon_text.cpp` | fork-only |
| `src/app/reader/views/sleep_screen.cpp` | different |
| `src/app/reader/views/sleep_screen.h` | different |

## tests
| Файл | Статус |
|---|---|
| `tests/desktop.cpp` | different |
| `tests/desktop_perf.h` | different |
| `tests/fixtures/fonts/fira-sans/FiraSans-Regular.ttf` | fork-only |
| `tests/fixtures/fonts/fira-sans/OFL.txt` | fork-only |
| `tests/fixtures/fonts/fira-sans/README.md` | fork-only |
| `tests/kobo_shutdown.cpp` | different |
| `tests/manual/README.md` | fork-only |
| `tests/manual/books/comics/README.md` | fork-only |
| `tests/manual/books/comics/spice-single-page.epub` | fork-only |
| `tests/manual/books/publisher/Кокаиновые+короли.epub` | fork-only |
| `tests/manual/materials.sha256` | fork-only |
| `tests/model/alternate_layout.cpp` | different |
| `tests/model/annotation_draft.cpp` | different |
| `tests/model/appearance_settings.cpp` | different |
| `tests/model/beautifier.cpp` | different |
| `tests/model/columns.cpp` | different |
| `tests/model/comics.cpp` | fork-only |
| `tests/model/dictionary_store.cpp` | different |
| `tests/model/document_cache.cpp` | different |
| `tests/model/engine_contract.cpp` | different |
| `tests/model/engine_contracts.cpp` | fork-only |
| `tests/model/engine_worker.cpp` | fork-only |
| `tests/model/fallback_styles.cpp` | different |
| `tests/model/fb2.cpp` | fork-only |
| `tests/model/fb2_session.cpp` | fork-only |
| `tests/model/figure_layout.cpp` | different |
| `tests/model/highlight_session.cpp` | different |
| `tests/model/large_toc.cpp` | different |
| `tests/model/links.cpp` | different |
| `tests/model/local_resources.cpp` | different |
| `tests/model/location_round_trip.cpp` | different |
| `tests/model/multilingual.cpp` | different |
| `tests/model/native_dialog_overlay.cpp` | different |
| `tests/model/nickel_book_menu.cpp` | different |
| `tests/model/publisher_markup.cpp` | different |
| `tests/model/reader.cpp` | different |
| `tests/model/reader_session.cpp` | different |
| `tests/model/reading_book.cpp` | different |
| `tests/model/reading_progress.cpp` | different |
| `tests/model/resource_cache.cpp` | different |
| `tests/model/search.cpp` | different |
| `tests/model/strings_locale.cpp` | fork-only |
| `tests/model/text_presets.cpp` | different |
| `tests/model/ui.cpp` | different |
| `tests/model/vertical_writing.cpp` | different |
| `tests/model/word_selection.cpp` | different |
| `tests/pewter/keyboard.cpp` | different |
| `tests/pewter/nickel.cpp` | different |
| `tests/pewter/nickel.h` | different |
| `tests/pewter/progress.cpp` | different |
| `tests/reader_lifecycle.cpp` | different |
| `tests/reader_ui.cpp` | different |
| `tests/support/anchor_preview.h` | fork-only |
| `tests/support/reader_source_index_probe.h` | different |
| `tests/support/screenshot_gallery.h` | different |
