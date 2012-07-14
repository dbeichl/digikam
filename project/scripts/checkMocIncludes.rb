#! /usr/bin/env ruby

# ============================================================
# 
# This file is a part of digiKam project
# http://www.digikam.org
# 
# Date        : 2012-07-14
# Description : a helper script for finding source code with no moc includes
# 
# Copyright (C) 2012 by Andi Clemens <andi dot clemens at googlemail dot com>
# 
# This program is free software; you can redistribute it
# and/or modify it under the terms of the GNU General
# Public License as published by the Free Software Foundation;
# either version 2, or (at your option)
# any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# ============================================================ */


# get all header files containing the line "Q_OBJECT"
candidates = Dir.glob("**/*.h").select do |file_name|
  File.read(file_name) =~ /^\s*Q_OBJECT\s*$/ rescue false
end

# get all source files with missing MOC includes
missingMocIncludes = candidates.select do |file_name|
  source_file = file_name.gsub(/\.h$/, ".cpp")
  moc_file = file_name.gsub(/\.h$/, ".moc").split("/").last
  pattern = /#include\s+[<"]#{moc_file}[>"]/

  begin
    File.open(source_file, "r") do |fp|
      fp.none? { |line| line =~ pattern }
    end
  rescue
    false
  end
end

# display missing MOC includes
puts "missing MOC include:"
widthStr = "#{missingMocIncludes.length}"

missingMocIncludes.each_with_index do |f, idx|
  puts "%#{widthStr.length}s: #{f}" % (idx + 1)
end
