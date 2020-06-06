/*
 * header_editor.h
 *
 *  Created on: Jun 5, 2020
 *      Author: rt135
 */

#ifndef INCLUDE_HEADER_EDITOR_H_
#define INCLUDE_HEADER_EDITOR_H_

#include <string>

#include <QCheckBox>
#include <QDialog>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QStringList>
#include <QTextEdit>

class HeaderEditor : public QDialog
{
	Q_OBJECT

public:
	explicit HeaderEditor(QWidget * parent = nullptr);
	explicit HeaderEditor(std::string header, QWidget * parent = nullptr);

	auto header() -> std::string;
signals:
	auto UpdateStatusBar() -> void;
public slots:
	auto UpdateFields() -> void;
private:
	auto CreateDialog() -> void;

	std::string header_;
	QStringList header_fields_;
	QWidget * parent_;
};



#endif /* INCLUDE_HEADER_EDITOR_H_ */
