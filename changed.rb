#!/usr/bin/env ruby

require 'optparse'
require 'ostruct'
require 'git'

options = OpenStruct.new
OptionParser.new do |opt|
  opt.on('-c', '--compare_with commit', 'The commit to diff the current commit with.') { |o| options.compare_with = o }
end.parse!

raise OptionParser::MissingArgument.new("compare_with") if options[:compare_with].nil?

# File.open(ENV["GITHUB_ENV"], 'a') { |f| f.write('TESTING_RUBY=iDidIt') }

g = Git.open('.')
d = g.diff(options.compare_with, 'HEAD')
files = d.stats[:files].keys

def any_match(files, pattern)
  return files.select{ |file| file[/\A#{pattern}\//]}.any?
end

ci_changed = any_match(files, "ci")
core_changed = any_match(files, "core")
projects_changed = any_match(files, "projects")

puts "CI changed #{ci_changed}"
puts "core changed #{core_changed}"
puts "projects changed #{projects_changed}"
