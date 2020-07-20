/*
 * header_editor.h
 *
 *  Created on: Jun 5, 2020
 *      Author: rt135
 */

#ifndef INCLUDE_HEADER_EDITOR_H_
#define INCLUDE_HEADER_EDITOR_H_

#include <fstream>
#include <string>

#include <QObject>
#include <QtCore/QtGlobal>

#if defined(HEADER_EDITOR_LIBRARY)
#  define HEADER_EDITOR_EXPORT Q_DECL_EXPORT
#else
#  define HEADER_EDITOR_EXPORT Q_DECL_IMPORT
#endif

// We forward declare the GUI, but we do not export it, thus abstracting it
class UserInterface;

class HEADER_EDITOR_EXPORT HeaderEditor : public QObject
{
	Q_OBJECT

public:
	explicit HeaderEditor(QWidget * parent = nullptr);
	explicit HeaderEditor(std::string header, QWidget * parent = nullptr);

	auto header() -> std::string;
signals:
public slots:
private:
  auto ExtractHeader(std::string filename) -> void;

  UserInterface * ui_;

	std::string header_;
  QStringList header_fields_;
};

#endif /* INCLUDE_HEADER_EDITOR_H_ */
